# Changelog

Firmware library versions. Each release's source code is on [GitHub](https://github.com/advancedbotworks/abw-sensors/releases).

## 3.0.0 (unreleased)

**For teams:** no change to how the sensors behave or what the R/G/B channels mean.

**For developers (breaking):**

- Library renamed to **AdvancedBotWorks**, with a single include: `#include <AdvancedBotWorks.h>`
- Classes and methods renamed to Arduino conventions: `ColourSensor`, `IRRing`, `getDirectionSimple()`, `getStrength()`, `getDirectionAdvanced()`
- Pololu's VL53L1X library is now a dependency rather than a bundled copy
- Added third-party licence notices and automatic build checks

## 2.0.0 (2025-03-07)

- Added the Laser Distance Sensor Array firmware

## 1.0.0 (2024-06-04)

- First release: IR Sensor Ring firmware (originally in a separate repository, now [archived](https://github.com/advancedbotworks/abw-ir-ring))
