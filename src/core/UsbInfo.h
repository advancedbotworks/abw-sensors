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
#ifndef ABW_USBINFO_H
#define ABW_USBINFO_H

namespace abw {

// Prints the product name and firmware version over USB each time a Serial
// Monitor connects (any baud rate). Call it every loop(): it only checks a
// flag, so it's cheap, and it doesn't use `if (Serial)`, which waits 10 ms
// on SAMD boards.
//
// Example output:
//   Advanced Bot Works IR Sensor Ring
//   Firmware v3.0.0 (AdvancedBotWorks library), hardware v1.0
//   Docs: https://docs.advancedbotworks.com
void printInfoOnUsbConnect(const char* productName);

} // namespace abw

#endif // ABW_USBINFO_H
