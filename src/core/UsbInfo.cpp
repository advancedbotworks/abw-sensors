/*
 * Advanced Bot Works Sensors
 * Copyright (C) 2026 Aparaj Bogahawatta
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
#include "UsbInfo.h"

#include <Arduino.h>
#include "../AdvancedBotWorksVersion.h"

namespace abw {

void printInfoOnUsbConnect(const char *productName) {
    static bool started = false;
    static bool wasConnected = false;

    if (!started) {
        Serial.begin(115200); // USB serial: the baud rate is ignored
        started = true;
    }

    // DTR goes high when a Serial Monitor opens the port.
    const bool connected = Serial.dtr();
    if (connected && !wasConnected) {
        Serial.print("Advanced Bot Works ");
        Serial.println(productName);
        Serial.print("Firmware v" ABW_VERSION " (AdvancedBotWorks library), hardware v");
        Serial.print(ABW_HARDWARE_VERSION_MAJOR);
        Serial.print('.');
        Serial.println(ABW_HARDWARE_VERSION_MINOR);
        Serial.println("Docs: https://docs.advancedbotworks.com");
    }
    wasConnected = connected;
}

} // namespace abw
