/*
 * Advanced Bot Works Sensors
 * Copyright (C) 2026 Aparaj Bogahawatta
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Library / firmware version. Keep in step with `version=` in
 * library.properties (CI checks this: make -C extras/test check-version).
 */
#ifndef ABW_ADVANCEDBOTWORKSVERSION_H
#define ABW_ADVANCEDBOTWORKSVERSION_H

#define ABW_VERSION_MAJOR 3 // 0-9  (LEGO version field is one BCD digit)
#define ABW_VERSION_MINOR 0 // 0-9  (one BCD digit)
#define ABW_VERSION_PATCH 0 // 0-99 (two BCD digits)
#define ABW_VERSION       "3.0.0"

// Hardware revision reported to the hub (major.minor). All current
// products are hardware v1.
#define ABW_HARDWARE_VERSION_MAJOR 1
#define ABW_HARDWARE_VERSION_MINOR 0

#endif // ABW_ADVANCEDBOTWORKSVERSION_H
