# How the sensors talk to the hub

ABW sensors don't need any special software on the SPIKE hub, because they **pretend to be an official LEGO® SPIKE™ colour sensor**. This page explains how that works. It's for curious and advanced users; you don't need any of it to use the sensors.

## The LEGO UART protocol

SPIKE Prime sensors talk to the hub over a serial (UART) link on the port's data pins. The protocol is often called **LPF2** or **LUMP**:

1. **Handshake.** When a device is plugged in, it introduces itself at 2400 baud: its **type ID** (the colour sensor is `0x3D`), how many **modes** it has, and the name, range, units and data format of each mode.
2. **Speed change.** The device and the hub then switch to 115200 baud.
3. **Data.** The hub picks a mode, and the device sends readings for that mode. The hub sends a regular "keep-alive" message; if the device stops hearing it, it resets and repeats the handshake.

## How ABW uses it

The ABW firmware (in `src/core/`) gives the full colour sensor handshake, so the hub and the SPIKE app treat the board as a genuine colour sensor. The sensor's own data is placed in the colour sensor's **RGB_I mode** (mode 5), which normally carries the raw red, green, blue and intensity values. That's why:

- your data shows up on the **red, green and blue** channels;
- each channel carries a 16-bit whole number;
- any software that can read a colour sensor's raw RGB values can read ABW sensors.

## Credits

The protocol code is based on the open-source [MyOwnBricks](https://github.com/ysard/MyOwnBricks) project by Ysard, and uses protocol definitions from [Pybricks](https://github.com/pybricks/pybricks-micropython). The protocol itself is documented by the Pybricks team in [pybricks/technical-info](https://github.com/pybricks/technical-info/blob/master/uart-protocol.md). See [Licences](../licences.md).
