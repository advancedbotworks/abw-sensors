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
 * Special values (sent instead of a distance):
 *   ABW_LASER_NO_TARGET    (9999)  nothing within range / reading not valid
 *   ABW_LASER_SENSOR_ERROR (65535) that sensor failed to start or stopped responding
 *
 * Requires Pololu's VL53L1X library (installed automatically by the
 * Arduino Library Manager as a dependency).
 */
#include <VL53L1X.h>
#include <AdvancedBotWorks.h>

// ============================== Settings ===================================

// Distance mode (sensor datasheet figures, white target):
//   VL53L1X::Short   about 1.3 m, same in the dark and in bright light (default)
//   VL53L1X::Medium  about 2.9 m in the dark, about 0.8 m in bright light
//   VL53L1X::Long    about 3.6 m in the dark, about 0.7 m in bright light
#define ABW_LASER_DISTANCE_MODE VL53L1X::Short

// Measurement period in ms. Used for both the timing budget and the
// inter-measurement period, so each sensor updates at 1000 / period Hz.
// Minimum 20 ms in Short mode, 33 ms in Medium/Long.
#define ABW_LASER_PERIOD_MS 33

// Smoothing: exponential moving average factor, 0 < alpha <= 1.
// 1.0 = no smoothing (default). Lower = smoother but slower to react.
#define ABW_LASER_SMOOTHING_ALPHA 1.0f

// Report distances in centimetres instead of millimetres.
#define ABW_LASER_CENTIMETRES false

// Print readings over USB (Serial Monitor, 115200) for testing.
#define ABW_LASER_DEBUG false

// ===========================================================================

#define SENSOR_COUNT           3
#define ABW_LASER_NO_TARGET    9999
#define ABW_LASER_SENSOR_ERROR 65535
#define ABW_LASER_STALE_MS     500 // no new reading for this long = error

const uint8_t xshutPins[SENSOR_COUNT] = {16, 14, 15}; // left, front, right

uint16_t data[4] = {0};

ColourSensor hub;
VL53L1X sensors[SENSOR_COUNT];
bool sensorOk[SENSOR_COUNT] = {false};
float smoothed[SENSOR_COUNT] = {0};
bool hasReading[SENSOR_COUNT] = {false};
unsigned long lastReadingMs[SENSOR_COUNT] = {0};

static bool isValidRange(VL53L1X::RangeStatus status) {
    return status == VL53L1X::RangeValid ||
           status == VL53L1X::RangeValidMinRangeClipped ||
           status == VL53L1X::RangeValidNoWrapCheckFail;
}

static uint16_t toOutput(float mm) {
#if ABW_LASER_CENTIMETRES
    return (uint16_t)(mm / 10.0f + 0.5f);
#else
    return (uint16_t)(mm + 0.5f);
#endif
}

void setup() {
#if ABW_LASER_DEBUG
    Serial.begin(115200);
#endif
    hub.setSensorRGB_I(data);
    Wire.begin();
    Wire.setClock(400000);

    // Hold every sensor in reset, then bring them up one at a time and give
    // each a unique I2C address.
    for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
        pinMode(xshutPins[i], OUTPUT);
        digitalWrite(xshutPins[i], LOW);
    }

    for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
        pinMode(xshutPins[i], INPUT);
        delay(10);
        sensors[i].setTimeout(500);

        if (!sensors[i].init()) {
            // Don't hang: report an error on this channel and keep going.
            sensorOk[i] = false;
            data[i] = ABW_LASER_SENSOR_ERROR;
            continue;
        }

        sensors[i].setAddress(0x2A + i);
        sensors[i].setDistanceMode(ABW_LASER_DISTANCE_MODE);
        sensors[i].setMeasurementTimingBudget(ABW_LASER_PERIOD_MS * 1000UL);
        sensors[i].startContinuous(ABW_LASER_PERIOD_MS);
        sensorOk[i] = true;
        lastReadingMs[i] = millis();
    }
}

void loop() {
    unsigned long now = millis();

    for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
        if (!sensorOk[i]) {
            data[i] = ABW_LASER_SENSOR_ERROR;
            continue;
        }

        if (sensors[i].dataReady()) {
            uint16_t mm = sensors[i].read(false); // non-blocking
            lastReadingMs[i] = now;

            if (sensors[i].timeoutOccurred()) {
                data[i] = ABW_LASER_SENSOR_ERROR;
            } else if (!isValidRange(sensors[i].ranging_data.range_status)) {
                data[i] = ABW_LASER_NO_TARGET;
                hasReading[i] = false; // restart smoothing on the next valid reading
            } else {
                if (!hasReading[i] || ABW_LASER_SMOOTHING_ALPHA >= 1.0f) {
                    smoothed[i] = mm;
                    hasReading[i] = true;
                } else {
                    smoothed[i] += ABW_LASER_SMOOTHING_ALPHA * ((float)mm - smoothed[i]);
                }
                data[i] = toOutput(smoothed[i]);
            }
        } else if (now - lastReadingMs[i] > ABW_LASER_STALE_MS) {
            data[i] = ABW_LASER_SENSOR_ERROR;
        }
    }

#if ABW_LASER_DEBUG
    static unsigned long lastPrintMs = 0;
    if (now - lastPrintMs >= 100) {
        lastPrintMs = now;
        Serial.print("L ");
        Serial.print(data[0]);
        Serial.print("  F ");
        Serial.print(data[1]);
        Serial.print("  R ");
        Serial.println(data[2]);
    }
#endif

    abw::printInfoOnUsbConnect("Laser Distance Sensor Array"); // version info for support (USB Serial Monitor)
    hub.process();
}
