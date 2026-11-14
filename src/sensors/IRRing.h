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
#ifndef ABW_IRRING_H
#define ABW_IRRING_H

#include <Arduino.h>
#include "IRRingMath.h"   // ABW_IR_SENSOR_COUNT, output values, the maths

/*
 * IR Sensor Ring driver: 12 x Vishay TSSP77038 receivers, 30 degrees apart.
 *
 * How it works
 * ------------
 * The TSSP demodulates the ball's 40 kHz carrier internally. Its output is
 * active-low only while a burst is present, so what matters is the FRACTION
 * OF TIME each output is low over a fixed window, not any single read.
 *
 * 1. A hardware timer (TC3) interrupts every ABW_IR_SAMPLE_PERIOD_US and reads
 *    all 12 outputs at once (two GPIO port register reads), counting low
 *    samples per receiver.
 * 2. After ABW_IR_SAMPLES_PER_WINDOW samples (one window = a whole number of
 *    ball burst periods, so the result doesn't depend on burst phase) the
 *    counts are latched and reset inside the interrupt.
 * 3. update() (called from loop) turns each count into a 0-100 strength,
 *    normalised by the measured duty ceiling, and smooths it with an
 *    exponential moving average (EMA).
 * 4. Outputs are calculated from the smoothed values:
 *    - strength: average of the strongest receiver and its two neighbours
 *      (doesn't dip when the ball sits between two receivers)
 *    - advanced direction: vector sum (centroid) of the strongest receiver
 *      and ABW_IR_DEFAULT_VECTOR_SPAN neighbours each side, or the older
 *      neighbour interpolation (setDirectionMethod)
 *
 * Ball: RoboCup Junior IR golf ball (github.com/robocup-junior/ir-golf-ball)
 *   carrier 40 kHz, 8 pulses per burst, burst rate 1.2 kHz
 *   -> burst period T_b = 833.3 us, burst on-time t_on = 200 us
 *   -> duty ceiling D = t_on / T_b = 0.24
 *
 * NOTE: this driver uses the TC3 timer and its interrupt handler. Don't use
 * libraries or pins that also need TC3 in the same sketch.
 */

// ---- Timing (derived from the ball specification, see above) ----
// Sample period: <= t_on / 10 so each 200 us burst gets ~10 samples.
#define ABW_IR_SAMPLE_PERIOD_US 20
// Window = k burst periods, k = 12 -> 10 ms -> 100 updates per second.
// Samples per window = 10 ms / 20 us.
#define ABW_IR_SAMPLES_PER_WINDOW 500

// ---- Defaults (can be changed at runtime with the setters below) ----
// Measured duty ceiling D_meas: the raw low fraction seen with the ball
// point-blank on one receiver. Theoretical value is 0.24; calibrate with the
// debug output and update this.
#define ABW_IR_DEFAULT_DUTY_MAX 0.24f
// EMA factor (0 < alpha <= 1). 0.4 at 100 Hz ~ 20 ms time constant.
#define ABW_IR_DEFAULT_SMOOTHING 0.4f
// Below this strength (0-100) the ball counts as "not detected".
#define ABW_IR_DEFAULT_NO_BALL_THRESHOLD 3.0f
// Strength scaling: the 3-receiver average that should read as 100
// (calibrate with the ball point-blank; 100 = no scaling).
#define ABW_IR_DEFAULT_STRENGTH_MAX 100.0f
// Vector-sum direction: receivers each side of the strongest to include
// (1-6; 6 = whole ring). Limiting it ignores far-side reflections/noise.
#define ABW_IR_DEFAULT_VECTOR_SPAN 2


class IRRing {
public:
    enum DirectionMethod {
        DIRECTION_VECTOR_SUM = abw::IR_DIRECTION_VECTOR_SUM,       // centroid of nearby receivers (default)
        DIRECTION_INTERPOLATION = abw::IR_DIRECTION_INTERPOLATION  // v2-style neighbour interpolation
    };

    IRRing() {}

    // Configure the pins and start the sampling timer. Call once in setup().
    void init();

    // Process the latest window, if one is ready. Call often from loop().
    // Returns true when new values were calculated (about 100 times a second).
    bool update();

    // ---- Outputs ----
    uint8_t  getDirectionSimple();    // 1-12, or 0 when no ball
    uint8_t  getStrength();           // 0-100, average of strongest + 2 neighbours
    uint16_t getDirectionAdvanced();  // 0-359 degrees, or 450 when no ball
    bool     isBallDetected();

    // ---- Settings ----
    void setDutyMax(float dutyMax);           // calibration, see ABW_IR_DEFAULT_DUTY_MAX
    void setSmoothing(float alpha);           // 0 < alpha <= 1 (1 = off)
    void setNoBallThreshold(float strength);  // 0-100
    void setStrengthMax(float average);       // calibration, see ABW_IR_DEFAULT_STRENGTH_MAX
    void setDirectionMethod(DirectionMethod method);
    void setVectorSpan(uint8_t span);         // 1-6

    // ---- Diagnostics (calibration / debugging) ----
    uint16_t getRawCount(uint8_t sensor);     // low samples in the last window (0-500)
    float    getRawFraction(uint8_t sensor);  // getRawCount / ABW_IR_SAMPLES_PER_WINDOW
    float    getSmoothed(uint8_t sensor);     // smoothed strength 0-100
    bool     isPinMapValid();                 // false = wrong board/variant; outputs stay "no ball"

    // Called from the timer interrupt. Not for use in sketches.
    static void sampleFromISR();

private:
    void calculateOutputs();

    float m_dutyMax = ABW_IR_DEFAULT_DUTY_MAX;
    float m_alpha = ABW_IR_DEFAULT_SMOOTHING;
    abw::IRRingSettings m_settings = {
        ABW_IR_DEFAULT_STRENGTH_MAX,
        ABW_IR_DEFAULT_NO_BALL_THRESHOLD,
        abw::IR_DIRECTION_VECTOR_SUM,
        ABW_IR_DEFAULT_VECTOR_SPAN};

    uint16_t m_rawCounts[ABW_IR_SENSOR_COUNT] = {0};
    float m_smoothed[ABW_IR_SENSOR_COUNT] = {0};

    uint8_t  m_directionSimple = ABW_IR_NO_BALL_DIRECTION;
    uint8_t  m_strength = 0;
    uint16_t m_directionAdvanced = ABW_IR_NO_BALL_ANGLE;
    bool     m_ballDetected = false;
};

#endif // ABW_IRRING_H
