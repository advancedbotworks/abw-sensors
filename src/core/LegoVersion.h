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
#ifndef ABW_LEGOVERSION_H
#define ABW_LEGOVERSION_H

/*
 * Builds the LEGO UART (LPF2/LUMP) CMD_VERSION message that a device sends
 * to the hub during its handshake. Header-only and hardware-free, so it can
 * be unit-tested on a computer (extras/test).
 *
 * Version format (32-bit, BCD, sent little-endian):
 *   major (1 digit) . minor (1 digit) . bug-fix (2 digits) . build (4 digits)
 *   e.g. 3.0.00.0000 -> 0x30000000
 *
 * Message (10 bytes):
 *   0x5F | firmware version (4) | hardware version (4) | checksum
 *   0x5F = system message (0x40) | CMD_VERSION (0x0F) | 8-byte payload (0x10)
 *   checksum = 0xFF XOR every preceding byte
 */

#include <stdint.h>

namespace abw {

const uint8_t kLegoVersionHeader = 0x5F;
const uint8_t kLegoVersionMessageSize = 10;

inline uint32_t legoVersion(uint8_t major, uint8_t minor, uint8_t bugfix) {
    return ((uint32_t)(major % 10) << 28) |
           ((uint32_t)(minor % 10) << 24) |
           ((uint32_t)((bugfix / 10) % 10) << 20) |
           ((uint32_t)(bugfix % 10) << 16);
}

inline void buildLegoVersionMessage(uint8_t out[kLegoVersionMessageSize],
                                    uint32_t firmwareVersion,
                                    uint32_t hardwareVersion) {
    out[0] = kLegoVersionHeader;
    for (uint8_t i = 0; i < 4; i++) {
        out[1 + i] = (uint8_t)(firmwareVersion >> (8 * i));
        out[5 + i] = (uint8_t)(hardwareVersion >> (8 * i));
    }
    uint8_t checksum = 0xFF;
    for (uint8_t i = 0; i < kLegoVersionMessageSize - 1; i++) {
        checksum ^= out[i];
    }
    out[kLegoVersionMessageSize - 1] = checksum;
}

} // namespace abw

#endif // ABW_LEGOVERSION_H
