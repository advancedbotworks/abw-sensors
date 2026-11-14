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
#include "IRRingMath.h"

#include <math.h>

namespace abw {

namespace {

const uint8_t kCount = ABW_IR_SENSOR_COUNT;
const float kDegreesPerReceiver = 360.0f / ABW_IR_SENSOR_COUNT;
const float kRadToDeg = 57.29577951f;

// Unit vectors for receivers 0..11 at 0, 30, ..., 330 degrees.
const float kCos[ABW_IR_SENSOR_COUNT] = {
    1.0f, 0.8660254f, 0.5f, 0.0f, -0.5f, -0.8660254f,
    -1.0f, -0.8660254f, -0.5f, 0.0f, 0.5f, 0.8660254f};
const float kSin[ABW_IR_SENSOR_COUNT] = {
    0.0f, 0.5f, 0.8660254f, 1.0f, 0.8660254f, 0.5f,
    0.0f, -0.5f, -0.8660254f, -1.0f, -0.8660254f, -0.5f};

uint8_t wrapIndex(int index) {
    return (uint8_t)(((index % kCount) + kCount) % kCount);
}

float clamp100(float value) {
    if (value > 100.0f) return 100.0f;
    if (value < 0.0f) return 0.0f;
    return value;
}

} // namespace

float irCountToStrength(uint16_t count, uint16_t samplesPerWindow, float dutyMax) {
    if (samplesPerWindow == 0 || dutyMax <= 0.0f) {
        return 0.0f;
    }
    const float fraction = (float)count / samplesPerWindow;
    return clamp100(100.0f * fraction / dutyMax);
}

float irSmooth(float previous, float input, float alpha) {
    return previous + alpha * (input - previous);
}

uint8_t irStrongest(const float values[ABW_IR_SENSOR_COUNT]) {
    uint8_t best = 0;
    for (uint8_t i = 1; i < kCount; i++) {
        if (values[i] > values[best]) {
            best = i;
        }
    }
    return best;
}

float irStrength(const float values[ABW_IR_SENSOR_COUNT], uint8_t best, float strengthMax) {
    // When the ball sits between two receivers one drops and the other rises,
    // so this average stays steady where the single maximum would dip.
    const float average = (values[wrapIndex(best - 1)] + values[best] +
                           values[wrapIndex(best + 1)]) /
                          3.0f;
    if (strengthMax <= 0.0f) {
        return clamp100(average);
    }
    return clamp100(100.0f * average / strengthMax);
}

float irPositionVectorSum(const float values[ABW_IR_SENSOR_COUNT], uint8_t best, uint8_t span) {
    float x = 0.0f;
    float y = 0.0f;

    if (span >= kCount / 2) {
        // Whole ring.
        for (uint8_t i = 0; i < kCount; i++) {
            x += values[i] * kCos[i];
            y += values[i] * kSin[i];
        }
    } else {
        // Strongest receiver and `span` neighbours each side: ignores
        // reflections and noise on the far side of the ring.
        for (int k = -(int)span; k <= (int)span; k++) {
            const uint8_t i = wrapIndex(best + k);
            x += values[i] * kCos[i];
            y += values[i] * kSin[i];
        }
    }

    // Vectors cancelled out (e.g. opposite receivers equally bright):
    // fall back to the strongest receiver.
    if (x * x + y * y < 1e-6f) {
        return (float)best;
    }

    return atan2f(y, x) * kRadToDeg / kDegreesPerReceiver;
}

float irPositionInterpolation(const float values[ABW_IR_SENSOR_COUNT], uint8_t best) {
    // Shift from the strongest receiver towards whichever side is stronger,
    // using the neighbours one and two receivers away.
    const float bestValue = values[best];
    if (bestValue <= 0.0f) {
        return (float)best;
    }
    const float offset =
        ((values[wrapIndex(best + 1)] - values[wrapIndex(best - 1)]) +
         (values[wrapIndex(best + 2)] - values[wrapIndex(best - 2)]) / 2.0f) /
        (2.0f * bestValue);
    return (float)best + offset;
}

uint16_t irPositionToDegrees(float position) {
    while (position < 0.0f)
        position += kCount;
    while (position >= (float)kCount)
        position -= kCount;

    uint16_t degrees = (uint16_t)(position * kDegreesPerReceiver + 0.5f);
    if (degrees >= 360) degrees -= 360;
    return degrees;
}

IRRingOutputs irCalculateOutputs(const float values[ABW_IR_SENSOR_COUNT],
                                 const IRRingSettings &settings) {
    IRRingOutputs out;

    const uint8_t best = irStrongest(values);
    const float strength = irStrength(values, best, settings.strengthMax);

    out.strength = (uint8_t)(strength + 0.5f);
    out.ballDetected = strength >= settings.noBallThreshold && strength > 0.0f;

    if (!out.ballDetected) {
        out.directionSimple = ABW_IR_NO_BALL_DIRECTION;
        out.directionAdvanced = ABW_IR_NO_BALL_ANGLE;
        return out;
    }

    out.directionSimple = best + 1;

    const float position = (settings.method == IR_DIRECTION_VECTOR_SUM)
                               ? irPositionVectorSum(values, best, settings.vectorSpan)
                               : irPositionInterpolation(values, best);
    out.directionAdvanced = irPositionToDegrees(position);
    return out;
}

} // namespace abw
