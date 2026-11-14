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

void IRRing::init() {
    for(uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
        pinMode(m_pins[i], INPUT);
        m_xValues[i] = cosf(RAD_TO_DEG * (450.0f - i * 30.0f));
        m_yValues[i] = sinf(RAD_TO_DEG * (450.0f - i * 30.0f));
	}
}

void IRRing::update() {
	for(uint8_t i = 0; i < ABW_IR_READ_COUNT; i++) {
		for(uint8_t j = 0; j < ABW_IR_SENSOR_COUNT; j++) {
		    m_values[j] += 1 - digitalRead(m_pins[j]);
	    }
	}
    for(uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
        for(uint8_t j = 0; j < ABW_IR_SENSOR_COUNT; j++) {
            if(m_values[i] > m_sortedValues[j]) {
                if(j <= i) {
                    ABW_ARRAY_SHIFT_DOWN(m_sortedValues, j, i);
                    ABW_ARRAY_SHIFT_DOWN(m_indexes, j, i);
                }
                m_sortedValues[j] = m_values[i];
                m_indexes[j] = i;
                break;
            }
		}
	}
    m_strength = m_sortedValues[0];
    m_directionSimple = (m_strength == 0) ? 0 : m_indexes[0] + 1;

    uint8_t index_lower_1 = (m_indexes[0] == 0) ? ABW_IR_SENSOR_COUNT - 1 : m_indexes[0] - 1;
    uint8_t index_upper_1 = (m_indexes[0] + 1) % 12;
    uint8_t index_lower_2 = (m_indexes[0] < 2) ? m_indexes[0] + ABW_IR_SENSOR_COUNT - 2 : m_indexes[0] - 2;
    uint8_t index_upper_2 = (m_indexes[0] + 2) % 12;

    float frac = (float)m_indexes[0] + ( ( (float)(m_values[index_upper_1] - m_values[index_lower_1]) + ((float)(m_values[index_upper_2] - m_values[index_lower_2]) / 2.0) )  /  (2.0 * (float)m_sortedValues[0]));
    frac = frac < 0 ? frac + 12.0 : frac;
    m_directionAdvanced = (m_strength == 0) ? 0 : round(frac * 30.0);

	for(uint8_t i = 0; i < ABW_IR_SENSOR_COUNT; i++) {
		m_values[i] = 0;
		m_sortedValues[i] = 0;
		m_indexes[i] = 0;
	}
}


uint16_t IRRing::getDirectionAdvanced() {
    return m_directionAdvanced;
}

uint8_t IRRing::getDirectionSimple() {
    return m_directionSimple;
}

uint8_t IRRing::getStrength() {
    return m_strength;
}
