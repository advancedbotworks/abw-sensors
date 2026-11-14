# AdvancedBotWorks: firmware for Advanced Bot Works sensors

[![Compile firmware](https://github.com/advancedbotworks/abw-sensors/actions/workflows/compile.yml/badge.svg)](https://github.com/advancedbotworks/abw-sensors/actions/workflows/compile.yml)
[![Unit tests](https://github.com/advancedbotworks/abw-sensors/actions/workflows/tests.yml/badge.svg)](https://github.com/advancedbotworks/abw-sensors/actions/workflows/tests.yml)
[![Docs](https://img.shields.io/badge/docs-docs.advancedbotworks.com-blue)](https://docs.advancedbotworks.com)
[![Licence: GPL-3.0-or-later](https://img.shields.io/badge/licence-GPL--3.0--or--later-green)](LICENSE)

The open-source Arduino library that runs on every [Advanced Bot Works](https://advancedbotworks.com) sensor for **LEGO® SPIKE™ Prime**. Each sensor appears to the hub as a SPIKE colour sensor and sends its data on the **red, green and blue** channels (plus a 4th "intensity" value), so teams can use it from Word Blocks, Python or Pybricks with no extra software.

> **Using a sensor in your robot?** You don't need anything from this repository. Head to **[docs.advancedbotworks.com](https://docs.advancedbotworks.com)** for setup, channel tables and example code.

## Products

| Product | Firmware | Docs |
|---|---|---|
| **IR Sensor Ring**: direction and strength of the RoboCup Junior Soccer IR ball, all the way around the robot | [`IRRingFirmware`](examples/IRRingFirmware) | [IR Sensor Ring](https://docs.advancedbotworks.com/products/ir-ring-v1/) |
| **Laser Distance Sensor Array**: millimetre distances to walls on the left, front and right, for Rescue Maze | [`LaserArrayFirmware`](examples/LaserArrayFirmware) | [Laser Distance Sensor Array](https://docs.advancedbotworks.com/products/laser-array-v1/) |

The sketches in [`examples/`](examples) are the exact firmware loaded onto each sensor before shipping.

## Versions

| | Version | Notes |
|---|---|---|
| **Latest release** | [**v2.0.0**](https://github.com/advancedbotworks/abw-sensors/releases/latest) | The firmware on all sensors sold so far. Use this to update or restore a sensor: see [Updating firmware](https://docs.advancedbotworks.com/firmware/updating/) |
| **`main` branch** | v3.0.0 (in development) | Faster, smoother IR sampling (100 Hz), a configurable laser distance mode, and real version reporting. **Changes what some channels report** (e.g. IR advanced direction = 450 when no ball). See the [changelog](https://docs.advancedbotworks.com/changelog/) |

Releases include **prebuilt firmware files** (`.bin`/`.hex`) for each sensor, with checksums.

## For developers

### Repository layout

```
src/
├── AdvancedBotWorks.h          # include this
├── AdvancedBotWorksVersion.h   # ABW_VERSION
├── core/                       # makes the board look like a SPIKE colour sensor to the hub
└── sensors/                    # sensor drivers (IRRing + the hardware-free IRRingMath)
examples/                       # the shipped firmware for each product
extras/test/                    # unit tests that run on a computer
docs/                           # source of docs.advancedbotworks.com (MkDocs)
```

### Building the firmware (v3 / `main`)

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) 2.3.4+ and, in the Boards Manager, **Arduino SAMD Boards**.
2. Put this repository in your Arduino `libraries` folder as a folder named **`AdvancedBotWorks`** (e.g. `git clone https://github.com/advancedbotworks/abw-sensors.git AdvancedBotWorks`).
3. Install the Laser Array's driver dependency from the Library Manager: **VL53L1X** by Pololu.
4. Select the board **Arduino NANO 33 IoT**, open **File → Examples → AdvancedBotWorks**, and upload.

Each firmware sketch has a **Settings** block at the top: calibration values, smoothing, laser distance mode and a **debug mode** that prints readings over USB. When a Serial Monitor connects, the sensor prints its product name and firmware version.

Minimal use of the library:

```cpp
#include <AdvancedBotWorks.h>

ColourSensor hub;
uint16_t data[4];          // R, G, B, I as seen by the hub

void setup() { hub.setSensorRGB_I(data); }

void loop() {
    data[0] = 123;         // ...your sensor values...
    hub.process();         // talk to the hub
}
```

More in the [developer guide](https://docs.advancedbotworks.com/firmware/library/) and [how the sensors talk to the hub](https://docs.advancedbotworks.com/firmware/protocol/).

### Tests, formatting and CI

```sh
make -C extras/test               # unit tests (IR maths, version message) + version check
make -C extras/test format        # apply clang-format 18 (pip install clang-format==18.1.8)
make -C extras/test format-check  # what CI checks
```

Every pull request is checked by GitHub Actions. It compiles both firmware sketches, runs Arduino Lint, the unit tests and the format check, and builds the docs when they change. `main` only accepts changes through pull requests that pass these checks.

## Contributing and support

- **Questions:** ask in [Discussions (Q&A)](https://github.com/advancedbotworks/abw-sensors/discussions/categories/q-a).
- **Bugs and ideas:** [open an issue](https://github.com/advancedbotworks/abw-sensors/issues/new/choose). The forms ask for the product, firmware version and readings.
- **Pull requests:** welcome. See [CONTRIBUTING.md](CONTRIBUTING.md) and our [Code of Conduct](CODE_OF_CONDUCT.md).
- **Orders, shipping or warranty:** email **support@advancedbotworks.com**. Please don't post order details on GitHub.

## Licence

Copyright © 2024–2026 Aparaj Bogahawatta.

Free software under the [GNU General Public License v3.0 or later](LICENSE). Parts are based on [MyOwnBricks](https://github.com/ysard/MyOwnBricks) (GPL-3.0-or-later) and [Pybricks](https://github.com/pybricks/pybricks-micropython) (MIT), and the Laser Array firmware uses a third-party driver library (BSD-3-Clause). See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) for full credits and licence texts.

<sub>LEGO® and SPIKE™ are trademarks of the LEGO Group, which does not sponsor or endorse this project.</sub>
