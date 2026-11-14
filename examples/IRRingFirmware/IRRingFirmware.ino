/*
 * IR Sensor Ring firmware - Advanced Bot Works
 * Copyright (C) 2024-2026 Aparaj Bogahawatta
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This is the firmware loaded onto every IR Sensor Ring before shipping.
 * The sensor appears to the SPIKE Prime hub as a colour sensor:
 *   Red   = simple direction (1-12, 0 = no ball)
 *   Green = signal strength (0-100): average of the strongest receiver and
 *           its two neighbours
 *   Blue  = advanced direction (0-359 degrees, 450 = no ball)
 *   4th   = ball detected (1) or not (0)  [the "intensity" value in rgbi()]
 *
 * Values update 100 times per second (see IRRing.h for how it works).
 */
#include <AdvancedBotWorks.h>

// ============================== Settings ===================================

// Calibration: raw low fraction with the ball point-blank on one receiver.
// Measure it with ABW_IR_DEBUG and put the value here (theory: 0.24).
#define ABW_IR_DUTY_MAX ABW_IR_DEFAULT_DUTY_MAX

// Smoothing (0 < alpha <= 1): lower = steadier, higher = faster to react.
#define ABW_IR_SMOOTHING ABW_IR_DEFAULT_SMOOTHING

// Strength (0-100) below which the ball counts as not detected.
#define ABW_IR_NO_BALL_THRESHOLD ABW_IR_DEFAULT_NO_BALL_THRESHOLD

// Calibration: the strength reading with the ball point-blank on one
// receiver (before scaling), so that point-blank reads 100.
#define ABW_IR_STRENGTH_MAX ABW_IR_DEFAULT_STRENGTH_MAX

// Advanced direction method:
//   IRRing::DIRECTION_VECTOR_SUM     centroid of nearby receivers (default)
//   IRRing::DIRECTION_INTERPOLATION  v2-style neighbour interpolation
#define ABW_IR_DIRECTION_METHOD IRRing::DIRECTION_VECTOR_SUM

// Vector sum only: receivers each side of the strongest to include
// (1-6, 6 = whole ring).
#define ABW_IR_VECTOR_SPAN ABW_IR_DEFAULT_VECTOR_SPAN

// Print raw counts, smoothed values and outputs over USB (Serial Monitor,
// 115200) for calibration and tuning. Set to true, upload, open the monitor.
#define ABW_IR_DEBUG false

// ===========================================================================

ColourSensor hub;
IRRing irRing;

uint16_t data[4] = {0, 0, 450, 0};

void setup() {
#if ABW_IR_DEBUG
    Serial.begin(115200);
#endif
    irRing.setDutyMax(ABW_IR_DUTY_MAX);
    irRing.setSmoothing(ABW_IR_SMOOTHING);
    irRing.setNoBallThreshold(ABW_IR_NO_BALL_THRESHOLD);
    irRing.setStrengthMax(ABW_IR_STRENGTH_MAX);
    irRing.setDirectionMethod(ABW_IR_DIRECTION_METHOD);
    irRing.setVectorSpan(ABW_IR_VECTOR_SPAN);
    irRing.init();
    hub.setSensorRGB_I(data);
}

void loop() {
    if (irRing.update()) {
        data[0] = irRing.getDirectionSimple();
        data[1] = irRing.getStrength();
        data[2] = irRing.getDirectionAdvanced();
        data[3] = irRing.isBallDetected() ? 1 : 0;

#if ABW_IR_DEBUG
        static uint8_t windows = 0;
        if (++windows >= 10) { // print 10 times per second
            windows = 0;
            if (!irRing.isPinMapValid()) {
                Serial.println("ERROR: receiver pin map doesn't match this board (expects Arduino NANO 33 IoT)");
            }
            Serial.print("raw:");
            for (uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
                Serial.print(' ');
                Serial.print(irRing.getRawCount(i));
            }
            Serial.print("  smooth:");
            for (uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
                Serial.print(' ');
                Serial.print(irRing.getSmoothed(i), 1);
            }
            Serial.print("  | dir ");
            Serial.print(data[0]);
            Serial.print("  str ");
            Serial.print(data[1]);
            Serial.print("  angle ");
            Serial.print(data[2]);
            Serial.print("  ball ");
            Serial.println(data[3]);
        }
#endif
    }
    abw::printInfoOnUsbConnect("IR Sensor Ring"); // version info for support (USB Serial Monitor)
    hub.process();
}
