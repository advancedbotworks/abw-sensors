# Library for developers

!!! note "This page describes v3.0.0 (coming soon)"
    The current release, **v2.0.0**, uses different names: the library is `Advanced_Bot_Works_Sensors`, and the class names also differ (see the v2.0.0 source). Browse the v2.0.0 source on the [releases page](https://github.com/advancedbotworks/abw-sensors/releases). The `main` branch on GitHub already has the v3 layout described below.

All the code that runs on ABW sensors is open source, in the **AdvancedBotWorks** Arduino library: [github.com/advancedbotworks/abw-sensors](https://github.com/advancedbotworks/abw-sensors). Advanced teams are welcome to read it, learn from it and adapt it for their own robots.

## Layout

```
src/
├── AdvancedBotWorks.h      # include this, and you get everything
├── core/                   # makes the board look like a SPIKE colour sensor
│   ├── BaseSensor.h/.cpp   # LEGO UART (LPF2) protocol handling
│   ├── ColourSensor.h/.cpp # colour sensor modes; your data goes out as R/G/B
│   └── lego_uart.h         # protocol constants (from Pybricks)
└── sensors/
    └── IRRing.h/.cpp       # IR Sensor Ring driver
examples/
├── IRRingFirmware/         # the exact firmware shipped on the IR Sensor Ring
└── LaserArrayFirmware/     # the exact firmware shipped on the Laser Array
```

## Minimal example

Every ABW firmware follows the same pattern: fill a 4-value array, give it to `ColourSensor`, and call `process()` in a loop.

```cpp
#include <AdvancedBotWorks.h>

ColourSensor hub;
uint16_t data[4];   // R, G, B, (unused)

void setup() {
    hub.setSensorRGB_I(data);   // these values are sent to the hub
}

void loop() {
    data[0] = 123;  // red channel
    data[1] = 456;  // green channel
    data[2] = 789;  // blue channel
    hub.process();  // talk to the hub: handshake, mode changes, sending data
}
```

## Naming conventions

| Thing | Convention | Example |
|---|---|---|
| Classes | PascalCase; file named after the class | `IRRing` in `IRRing.h` |
| Methods | camelCase | `getDirectionSimple()` |
| Private members | `m_` + camelCase | `m_directionSimple` |
| Public macros | `ABW_` + UPPER_SNAKE | `ABW_IR_SENSOR_COUNT` |
| Folders in `src/` | lowercase | `core/`, `sensors/` |

ABW code uses Australian spelling ("Colour"). Third-party code keeps its original names.

## Building

The firmware is built with the Arduino SAMD core, board **Arduino NANO 33 IoT** (`arduino:samd:nano_33_iot`). Every pull request is compiled automatically by GitHub Actions.

## Contributing

Found a bug or have an idea? [Open an issue](https://github.com/advancedbotworks/abw-sensors/issues). Pull requests are welcome too. By contributing, you agree that your contribution is licensed under the GPL-3.0-or-later.
