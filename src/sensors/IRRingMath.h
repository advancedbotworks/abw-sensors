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
#ifndef ABW_IRRINGMATH_H
#define ABW_IRRINGMATH_H

/*
 * IR Sensor Ring maths: everything that turns per-receiver sample counts
 * into the values sent to the hub.
 *
 * Deliberately free of Arduino and hardware code, so it can be compiled and
 * unit-tested on a normal computer (see extras/test). IRRing.cpp does the
 * hardware side (timer, GPIO) and calls these functions.
 *
 * Conventions:
 *   - Receivers are indexed 0..ABW_IR_SENSOR_COUNT-1; receiver i faces
 *     i * 30 degrees. Simple direction reports i + 1 (1-12).
 *   - "Strength" values are 0-100.
 */

#include <stdint.h>

#define ABW_IR_SENSOR_COUNT 12

// ---- Output values ----
#define ABW_IR_NO_BALL_DIRECTION 0   // simple direction when no ball
#define ABW_IR_NO_BALL_ANGLE     450 // advanced direction when no ball

namespace abw {

enum IRDirectionMethod {
    IR_DIRECTION_VECTOR_SUM,   // centroid of the receivers near the ball
    IR_DIRECTION_INTERPOLATION // v2-style neighbour interpolation
};

struct IRRingSettings {
    float strengthMax;     // 3-receiver average that reads as 100
    float noBallThreshold; // strength below this = no ball
    IRDirectionMethod method;
    uint8_t vectorSpan; // receivers each side of the strongest (1-6)
};

struct IRRingOutputs {
    uint8_t directionSimple;    // 1-12, or ABW_IR_NO_BALL_DIRECTION
    uint8_t strength;           // 0-100
    uint16_t directionAdvanced; // 0-359, or ABW_IR_NO_BALL_ANGLE
    bool ballDetected;
};

// Raw low-sample count over one window -> 0-100, normalised by the measured
// duty ceiling (the low fraction seen with the ball point-blank) and clamped.
float irCountToStrength(uint16_t count, uint16_t samplesPerWindow, float dutyMax);

// One step of an exponential moving average. alpha = 1 returns `input`.
float irSmooth(float previous, float input, float alpha);

// Index of the strongest receiver (first one wins a tie).
uint8_t irStrongest(const float values[ABW_IR_SENSOR_COUNT]);

// Average of the strongest receiver and the one each side, scaled by
// settings.strengthMax and clamped to 0-100 (not rounded).
float irStrength(const float values[ABW_IR_SENSOR_COUNT], uint8_t best, float strengthMax);

// Ball position in receivers (0 .. ABW_IR_SENSOR_COUNT, may need wrapping).
float irPositionVectorSum(const float values[ABW_IR_SENSOR_COUNT], uint8_t best, uint8_t span);
float irPositionInterpolation(const float values[ABW_IR_SENSOR_COUNT], uint8_t best);

// Position in receivers -> whole degrees 0-359.
uint16_t irPositionToDegrees(float position);

// All outputs from the smoothed per-receiver strengths.
IRRingOutputs irCalculateOutputs(const float values[ABW_IR_SENSOR_COUNT],
                                 const IRRingSettings &settings);

} // namespace abw

#endif // ABW_IRRINGMATH_H
