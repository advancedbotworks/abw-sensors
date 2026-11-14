/*
 * Laser Distance Sensor Array firmware - Advanced Bot Works
 * Copyright (C) 2025-2026 Aparaj Bogahawatta
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This is the firmware loaded onto every Laser Distance Sensor Array before
 * shipping. The sensor appears to the SPIKE Prime hub as a colour sensor:
 *   Red   = left distance (mm)
 *   Green = front distance (mm)
 *   Blue  = right distance (mm)
 *
 * Requires Pololu's VL53L1X library (installed automatically by the
 * Arduino Library Manager as a dependency).
 */
#include <VL53L1X.h>
#include <AdvancedBotWorks.h>

#define SENSOR_COUNT 3
#define CENTIMETRES false

const uint8_t xshutPins[SENSOR_COUNT] = {16, 14, 15};
uint16_t data[4] = {0};

ColourSensor hub;
VL53L1X sensors[SENSOR_COUNT];

void setup()
{
    hub.setSensorRGB_I(data);
    Wire.begin();
    Wire.setClock(400000);

    for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
        pinMode(xshutPins[i], OUTPUT);
        digitalWrite(xshutPins[i], LOW);
    }

    for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
        pinMode(xshutPins[i], INPUT);
        delay(10);
        sensors[i].setTimeout(500);

        if(!sensors[i].init()) {
            while (1);
        }

        sensors[i].setAddress(0x2A + i);
        sensors[i].startContinuous(33); // ~30Hz
    }
}

void loop()
{
    for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
        #if CENTIMETRES
            data[i] = (uint16_t)(sensors[i].read() / 10);
        #else
            data[i] = sensors[i].read();
        #endif
    }
    hub.process();
}
