/*
 * Advanced Bot Works Sensors
 * Copyright (C) 2024-2026 Aparaj Bogahawatta
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "IRRing.h"

#if !defined(ARDUINO_ARCH_SAMD)
#error "IRRing uses the TC3 timer and only supports the Arduino NANO 33 IoT board core."
#endif

namespace {

// IR receiver -> Arduino pin (Nano 33 IoT numbering), receiver 1..12 in order.
const uint8_t kPins[ABW_IR_SENSOR_COUNT] = {17, 14, 20, 2, 10, 9, 8, 3, 6, 21, 15, 18};

// The same receivers as GPIO port bits (from the Nano 33 IoT variant pin table).
// Fixed at compile time so the interrupt can test each bit with a constant
// mask. init() checks these against the board's pin table.
//   receiver:   1     2     3     4     5     6     7     8     9    10    11    12
//   pin:      PA10  PA02  PA09  PB10  PA21  PA20  PA18  PB11  PA04  PB03  PB02  PB08
const uint8_t kPortGroup[ABW_IR_SENSOR_COUNT] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1};
const uint8_t kPortBit[ABW_IR_SENSOR_COUNT]   = {10, 2, 9,10,21,20,18,11, 4, 3, 2, 8};

bool s_pinMapOk = false;

// Interrupt-side state.
uint16_t s_accumulators[ABW_IR_SENSOR_COUNT];
uint16_t s_sampleCount = 0;
volatile uint16_t s_latched[ABW_IR_SENSOR_COUNT];
volatile bool s_windowReady = false;

void startSampleTimer() {
    // Clock TC3 from GCLK0 (48 MHz).
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3;
    while (GCLK->STATUS.bit.SYNCBUSY);

    TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
    while (TC3->COUNT16.CTRLA.bit.SWRST);

    // 16-bit counter, reset on match with CC0 ("match frequency" mode), no prescaler.
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ | TC_CTRLA_PRESCALER_DIV1;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
    TC3->COUNT16.CC[0].reg = (uint16_t)((F_CPU / 1000000UL) * ABW_IR_SAMPLE_PERIOD_US - 1);
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);

    TC3->COUNT16.INTENSET.reg = TC_INTENSET_MC0;
    // Priority 1: above SysTick (2) and the hub UART/SERCOM (3), so samples are
    // evenly spaced. The handler is only a few microseconds long, so the UART
    // still has plenty of time to empty its buffer.
    NVIC_SetPriority(TC3_IRQn, 1);
    NVIC_EnableIRQ(TC3_IRQn);

    TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
}

} // namespace

extern "C" void TC3_Handler(void) {
    TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;   // clear the interrupt
    IRRing::sampleFromISR();
}

void IRRing::sampleFromISR() {
    // Read all receivers in the same instant: one read per GPIO port.
    // Outputs are active-low: a 0 bit means a ball burst is being received.
    const uint32_t lowA = ~PORT->Group[0].IN.reg;
    const uint32_t lowB = ~PORT->Group[1].IN.reg;

    // Unrolled with constant masks (must match kPortGroup / kPortBit).
    s_accumulators[0]  += (lowA >> 10) & 1U;   // receiver 1,  PA10
    s_accumulators[1]  += (lowA >> 2)  & 1U;   // receiver 2,  PA02
    s_accumulators[2]  += (lowA >> 9)  & 1U;   // receiver 3,  PA09
    s_accumulators[3]  += (lowB >> 10) & 1U;   // receiver 4,  PB10
    s_accumulators[4]  += (lowA >> 21) & 1U;   // receiver 5,  PA21
    s_accumulators[5]  += (lowA >> 20) & 1U;   // receiver 6,  PA20
    s_accumulators[6]  += (lowA >> 18) & 1U;   // receiver 7,  PA18
    s_accumulators[7]  += (lowB >> 11) & 1U;   // receiver 8,  PB11
    s_accumulators[8]  += (lowA >> 4)  & 1U;   // receiver 9,  PA04
    s_accumulators[9]  += (lowB >> 3)  & 1U;   // receiver 10, PB03
    s_accumulators[10] += (lowB >> 2)  & 1U;   // receiver 11, PB02
    s_accumulators[11] += (lowB >> 8)  & 1U;   // receiver 12, PB08

    if (++s_sampleCount >= ABW_IR_SAMPLES_PER_WINDOW) {
        // End of window: latch and reset together, so update() never sees a
        // half-finished window.
        for (uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
            s_latched[i] = s_accumulators[i];
            s_accumulators[i] = 0;
        }
        s_sampleCount = 0;
        s_windowReady = true;
    }
}

void IRRing::init() {
    for (uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
        // Receiver outputs are open-collector; the pull-up is harmless if the
        // board already has one.
        pinMode(kPins[i], INPUT_PULLUP);
        s_accumulators[i] = 0;
    }

    // Check the compile-time port map against the board's pin table.
    s_pinMapOk = true;
    for (uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
        if ((uint8_t)g_APinDescription[kPins[i]].ulPort != kPortGroup[i] ||
            (uint8_t)g_APinDescription[kPins[i]].ulPin  != kPortBit[i]) {
            s_pinMapOk = false;
        }
    }
    s_sampleCount = 0;
    s_windowReady = false;
    startSampleTimer();
}

bool IRRing::update() {
    bool ready = false;

    noInterrupts();
    if (s_windowReady) {
        for (uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
            m_rawCounts[i] = s_latched[i];
        }
        s_windowReady = false;
        ready = true;
    }
    interrupts();

    if (!ready) {
        return false;
    }

    for (uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
        // Fraction of the window the output was low, normalised to 0-100 by
        // the measured duty ceiling, then smoothed. (Maths in IRRingMath.cpp.)
        const float strength =
            abw::irCountToStrength(m_rawCounts[i], ABW_IR_SAMPLES_PER_WINDOW, m_dutyMax);
        m_smoothed[i] = abw::irSmooth(m_smoothed[i], strength, m_alpha);
    }

    calculateOutputs();
    return true;
}

void IRRing::calculateOutputs() {
    const abw::IRRingOutputs out = abw::irCalculateOutputs(m_smoothed, m_settings);

    m_strength = out.strength;
    // Wrong board (pin map mismatch): never report a ball.
    m_ballDetected = s_pinMapOk && out.ballDetected;
    m_directionSimple = m_ballDetected ? out.directionSimple : ABW_IR_NO_BALL_DIRECTION;
    m_directionAdvanced = m_ballDetected ? out.directionAdvanced : ABW_IR_NO_BALL_ANGLE;
}

uint8_t IRRing::getDirectionSimple()    { return m_directionSimple; }
uint8_t IRRing::getStrength()           { return m_strength; }
uint16_t IRRing::getDirectionAdvanced() { return m_directionAdvanced; }
bool IRRing::isBallDetected()           { return m_ballDetected; }

void IRRing::setDutyMax(float dutyMax) {
    if (dutyMax > 0.0f) m_dutyMax = dutyMax;
}

void IRRing::setSmoothing(float alpha) {
    if (alpha > 0.0f && alpha <= 1.0f) m_alpha = alpha;
}

void IRRing::setNoBallThreshold(float strength) {
    if (strength >= 0.0f && strength <= 100.0f) m_settings.noBallThreshold = strength;
}

void IRRing::setStrengthMax(float average) {
    if (average > 0.0f) m_settings.strengthMax = average;
}

void IRRing::setDirectionMethod(DirectionMethod method) {
    m_settings.method = (abw::IRDirectionMethod)method;
}

void IRRing::setVectorSpan(uint8_t span) {
    if (span >= 1 && span <= ABW_IR_SENSOR_COUNT / 2) m_settings.vectorSpan = span;
}

uint16_t IRRing::getRawCount(uint8_t sensor) {
    return sensor < ABW_IR_SENSOR_COUNT ? m_rawCounts[sensor] : 0;
}

float IRRing::getRawFraction(uint8_t sensor) {
    return (float)getRawCount(sensor) / ABW_IR_SAMPLES_PER_WINDOW;
}

bool IRRing::isPinMapValid() { return s_pinMapOk; }

float IRRing::getSmoothed(uint8_t sensor) {
    return sensor < ABW_IR_SENSOR_COUNT ? m_smoothed[sensor] : 0.0f;
}
