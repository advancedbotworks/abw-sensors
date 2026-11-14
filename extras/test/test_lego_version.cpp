/*
 * Advanced Bot Works Sensors - host unit tests for the LEGO CMD_VERSION message
 * Copyright (C) 2026 Aparaj Bogahawatta
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "LegoVersion.h"
#include "AdvancedBotWorksVersion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace abw;

static int g_failures = 0;
#define CHECK(cond)                                                  \
    do {                                                             \
        if (!(cond)) {                                               \
            g_failures++;                                            \
            printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                                                            \
    } while (0)

static void expectMessage(uint32_t fw, uint32_t hw, const uint8_t expected[10], const char *name) {
    uint8_t msg[kLegoVersionMessageSize];
    buildLegoVersionMessage(msg, fw, hw);
    bool ok = memcmp(msg, expected, 10) == 0;
    if (!ok) {
        g_failures++;
        printf("  FAIL %s: got", name);
        for (int i = 0; i < 10; i++)
            printf(" %02X", msg[i]);
        printf("\n");
    }
    printf("%s %s\n", ok ? "PASS" : "FAIL", name);
}

int main() {
    // BCD encoding
    CHECK(legoVersion(1, 0, 0) == 0x10000000u);
    CHECK(legoVersion(3, 0, 0) == 0x30000000u);
    CHECK(legoVersion(3, 1, 2) == 0x31020000u);
    CHECK(legoVersion(3, 0, 12) == 0x30120000u);
    CHECK(legoVersion(9, 9, 99) == 0x99990000u);

    // Must reproduce the original hard-coded message exactly (fw 1.0.0.0, hw 1.0.0.0).
    const uint8_t legacy[10] = {0x5F, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0xA0};
    expectMessage(legoVersion(1, 0, 0), legoVersion(1, 0, 0), legacy, "legacy_1.0.0_message_unchanged");

    // v3.0.0 firmware on v1.0 hardware.
    const uint8_t v300[10] = {0x5F, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x10, 0x80};
    expectMessage(legoVersion(3, 0, 0), legoVersion(1, 0, 0), v300, "v3.0.0_message");

    // Checksum: XOR of all 10 bytes with 0xFF seed must be 0.
    uint8_t msg[kLegoVersionMessageSize];
    buildLegoVersionMessage(msg, legoVersion(ABW_VERSION_MAJOR, ABW_VERSION_MINOR, ABW_VERSION_PATCH),
                            legoVersion(ABW_HARDWARE_VERSION_MAJOR, ABW_HARDWARE_VERSION_MINOR, 0));
    uint8_t x = 0xFF;
    for (int i = 0; i < 10; i++)
        x ^= msg[i];
    CHECK(x == 0);
    CHECK(msg[0] == 0x5F);

    // ABW_VERSION string matches the numeric parts.
    char built[16];
    snprintf(built, sizeof built, "%d.%d.%d", ABW_VERSION_MAJOR, ABW_VERSION_MINOR, ABW_VERSION_PATCH);
    CHECK(strcmp(built, ABW_VERSION) == 0);

    printf("%s lego_version_checks\n", g_failures ? "FAIL" : "PASS");
    printf("\n%d failures\n", g_failures);
    return g_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
