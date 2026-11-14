/*
 * Advanced Bot Works Sensors
 * Copyright (C) 2024-2026 Aparaj Bogahawatta
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Single include for the whole library:
 *
 *     #include <AdvancedBotWorks.h>
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
#ifndef ABW_ADVANCEDBOTWORKS_H
#define ABW_ADVANCEDBOTWORKS_H

// Library version (ABW_VERSION, e.g. "3.0.0")
#include "AdvancedBotWorksVersion.h"

// Shared core: emulates a LEGO SPIKE Prime colour sensor
#include "core/ColourSensor.h"

// Prints product name + firmware version when a Serial Monitor connects
#include "core/UsbInfo.h"

// Sensor drivers
#include "sensors/IRRing.h"

#endif // ABW_ADVANCEDBOTWORKS_H
