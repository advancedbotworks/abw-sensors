/*
 * IR Sensor Ring firmware - Advanced Bot Works
 * Copyright (C) 2024-2026 Aparaj Bogahawatta
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This is the firmware loaded onto every IR Sensor Ring before shipping.
 * The sensor appears to the SPIKE Prime hub as a colour sensor:
 *   Red   = simple direction (1-12, 0 = no ball)
 *   Green = signal strength (0-100)
 *   Blue  = advanced direction (0-360 degrees)
 */
#include <AdvancedBotWorks.h>

ColourSensor hub;
IRRing irRing;

uint16_t data[4];

void setup() {
    irRing.init();
    hub.setSensorRGB_I(data);
}

void loop() {
    irRing.update();
    data[0] = irRing.getDirectionSimple();
    data[1] = irRing.getStrength();
    data[2] = irRing.getDirectionAdvanced();
    hub.process();
}
