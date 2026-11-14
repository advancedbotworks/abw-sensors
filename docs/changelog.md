# Changelog

Firmware library versions. Each release's source code is on [GitHub](https://github.com/advancedbotworks/abw-sensors/releases).

## 3.0.0 (TBC: release date)

**For teams (behaviour changes).** See [Firmware versions](firmware/versions.md) for how to tell which version a sensor has, and what to change in your robot's code.

- **IR Sensor Ring:**
    - **Advanced direction (blue) is now 450 when no ball is detected** (it was 0, which is also a valid angle). Simple direction (red) is still 0 for no ball.
    - **Strength (green) is now the average of the strongest receiver and its two neighbours**, so it no longer dips when the ball is between two receivers.
    - **Advanced direction uses a vector sum (centroid)** of the nearby receivers, giving a smoother angle all the way around.
    - **New 4th value** (the "intensity" in `rgbi()`): **1 = ball detected, 0 = no ball**.
    - Rebuilt sampling: all 12 receivers are read together by a timer interrupt, and values update **100 times per second** with smoothing, so readings are steadier and more consistent.
    - Tuned for the RoboCup Junior IR golf ball (40 kHz, 8 pulses per burst at 1.2 kHz).
- **Laser Distance Sensor Array:**
    - Now uses the VL53L1X's **Short distance mode by default** (about 1.3 m in any lighting), at **30 readings per second** (previously Long mode at about 20 per second).
    - **9999** means no target in range; **65535** means a sensor error.

**For developers (breaking):**

- Library renamed to **AdvancedBotWorks**, with a single include: `#include <AdvancedBotWorks.h>`
- Classes and methods renamed to Arduino conventions: `ColourSensor`, `IRRing`, `getDirectionSimple()`, `getStrength()`, `getDirectionAdvanced()`
- Pololu's VL53L1X library is now a dependency rather than a bundled copy
- Added third-party licence notices and automatic build checks

## 2.0.0 (2025-03-07)

- Added the Laser Distance Sensor Array firmware

## 1.0.0 (2024-06-04)

- First release: IR Sensor Ring firmware (originally in a separate repository, now [archived](https://github.com/advancedbotworks/abw-ir-ring))
