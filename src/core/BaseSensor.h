/*
 * MyOwnBricks is a library for the emulation of PoweredUp sensors on microcontrollers
 * Copyright (C) 2021-2023 Ysard - <ysard@users.noreply.github.com>
 *
 * Based on the original work of Ahmed Jouirou - <ahmed.jouirou@gmail.com>
 *
 * Modified by Aparaj Bogahawatta for Advanced Bot Works, 2024-2026:
 * adapted to emulate a SPIKE Prime colour sensor on Advanced Bot Works
 * hardware, and files/classes renamed. See the git history for details.
 * Modifications Copyright (C) 2024-2026 Aparaj Bogahawatta
 *
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
#ifndef BASESENSOR_H
#define BASESENSOR_H

#include "lego_uart.h"
#include <Arduino.h>


/**
 * @brief Handle basic functions for LegoUART protocol.
 *      Designed to be inherited in specific classes of sensors.
 *
 * How a connection to the hub works (call process() every loop()):
 *   1. Not connected: process() calls connectToHub(), which
 *      - waits for the hub's TX line to stay idle (high) for 100 ms
 *        (commWaitForHubIdle(); this waits as long as it takes),
 *      - sends the device's description at 2400 baud
 *        (commSendInitSequence(): type ID, modes, speed, version, ...),
 *      - waits up to 2 s for the hub's ACK, then switches to 115200 baud.
 *   2. Connected: process() calls handleModes(), which answers the hub's
 *      mode selections and keep-alive messages and sends sensor data.
 *   3. If the hub goes quiet for more than 200 ms (e.g. unplugged),
 *      m_connected is cleared and the next process() starts again at 1.
 *
 * Note for ABW firmware: step 1 blocks loop() while no hub is connected.
 * Use setWaitCallback() to keep other work (e.g. USB output, sampling)
 * running while it waits.
 *
 * @param m_connSerialRX_pin Serial RX pin of the board. (default: 0).
 * @param m_connSerialTX_pin Serial TX pin of the board. (default: 1).
 * @param m_rxBuf Buffer used to store bytes emitted by the hub.
 * @param m_txBug Buffer used to store bytes before being sent to the hub.
 * @param m_lastAckTick Time flag used to detect disconnection from the hub.
 * @param m_connected Connection flag.
 */
class BaseSensor {
public:
    BaseSensor();
    // virtual ~BasicSensor(){}
    void process();
    bool isConnected();

    // Called repeatedly while waiting for a hub (connectToHub()), so a
    // sketch can keep doing other work, e.g. printing over USB when the
    // sensor is plugged into a computer without a hub. Keep it short
    // (well under 10 ms). Pass nullptr to remove it.
    void setWaitCallback(void (*callback)());

protected:
    // Protocol handy functions
    uint8_t calcChecksum(uint8_t *pData, int length);
    uint8_t getHeader(const lump_msg_type_t &msg_type, const uint8_t &mode, const uint8_t &msg_size);
    void parseHeader(const uint8_t &header, uint8_t &mode, uint8_t &msg_size);
    uint8_t getMsgSize(const uint8_t &header);
    void sendUARTBuffer(uint8_t msg_size);
    void commWaitForHubIdle();
    void connectToHub();
    // Could/should use virtual pure (..() = 0) but it uses 14bytes for nothing
    virtual void commSendInitSequence();
    virtual void handleModes();

    uint8_t m_connSerialRX_pin;
    uint8_t m_connSerialTX_pin;

    unsigned char m_rxBuf[16];
    unsigned char m_txBuf[16];
    unsigned long m_lastAckTick;

    bool m_connected;

    void (*m_waitCallback)() = nullptr;
    void runWaitCallback();
};

#endif // BASESENSOR_H
