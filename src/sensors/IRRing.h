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

// Config
#define ABW_IR_SENSOR_COUNT 12
#define ABW_IR_READ_COUNT 100

// TSSP receiver -> microcontroller pin connections
// (SAMD21 / Arduino Nano 33 IoT pin numbering)
#define ABW_IR_PIN_1 17
#define ABW_IR_PIN_2 14
#define ABW_IR_PIN_3 20
#define ABW_IR_PIN_4 2
#define ABW_IR_PIN_5 10
#define ABW_IR_PIN_6 9
#define ABW_IR_PIN_7 8
#define ABW_IR_PIN_8 3
#define ABW_IR_PIN_9 6
#define ABW_IR_PIN_10 21
#define ABW_IR_PIN_11 15
#define ABW_IR_PIN_12 18

// Shift the elements of array a in [lower, upper] down by one place
#define ABW_ARRAY_SHIFT_DOWN(a, lower, upper){    \
    if (upper == (sizeof(a)/sizeof(a[0])) - 1){   \
        for(int q = upper - 1; q >= lower; q--) { \
			*(a + q + 1) = *(a + q); }            \
	    } else {                                  \
		    for(int q = upper; q >= lower; q--) { \
			    *(a + q + 1) = *(a + q); }}}



class IRRing {
public:
    IRRing() {}
    void init();
    void update();
    uint16_t getDirectionAdvanced();
    uint8_t getDirectionSimple();
    uint8_t getStrength();
private:
    uint16_t m_directionAdvanced;
    uint8_t m_directionSimple;
    uint8_t m_strength;
    uint8_t m_pins[ABW_IR_SENSOR_COUNT] = {ABW_IR_PIN_1, ABW_IR_PIN_2, ABW_IR_PIN_3, ABW_IR_PIN_4, ABW_IR_PIN_5, ABW_IR_PIN_6, ABW_IR_PIN_7, ABW_IR_PIN_8, ABW_IR_PIN_9, ABW_IR_PIN_10, ABW_IR_PIN_11, ABW_IR_PIN_12};
    uint8_t m_values[ABW_IR_SENSOR_COUNT] = {0};
    uint8_t m_sortedValues[ABW_IR_SENSOR_COUNT] = {0};
    uint8_t m_indexes[ABW_IR_SENSOR_COUNT] = {0};
    float m_xValues[ABW_IR_SENSOR_COUNT] = {0};
    float m_yValues[ABW_IR_SENSOR_COUNT] = {0};
};

#endif // ABW_IRRING_H
