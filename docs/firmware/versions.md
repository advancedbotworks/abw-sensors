# Firmware versions

Every ABW sensor runs firmware from the open-source [AdvancedBotWorks library](library.md). Newer firmware can change what the R/G/B channels mean, so if your robot behaves unexpectedly, **check which version your sensor has**. The product pages show both versions side by side.

| Version | Released | Shipped on | Source code |
|---|---|---|---|
| **3.0.0** (current) | TBC | Sensors sold from TBC | [v3.0.0](https://github.com/advancedbotworks/abw-sensors/releases/tag/v3.0.0) |
| **2.0.0** | 2025-03-07 | All sensors sold before TBC | [v2.0.0](https://github.com/advancedbotworks/abw-sensors/releases/tag/v2.0.0) |
| 1.0.0 | 2024-06-04 | Early IR Sensor Rings | [archived repository](https://github.com/advancedbotworks/abw-ir-ring) |

## Which version does my sensor have?

You can tell from the readings. No computer is needed.

=== "IR Sensor Ring"

    Plug it into the hub, open the SPIKE app's device view, and make sure **no IR ball is switched on nearby**. Look at the **blue** value:

    | Blue value with no ball | Firmware |
    |---|---|
    | **450** | v3.0.0 or later |
    | **0** | v2.0.0 (or 1.0.0) |

=== "Laser Distance Sensor Array"

    Point one sensor at open space with nothing within about 1.5 m, e.g. up at a high ceiling or out of an open door, and look at that sensor's value:

    | Value with nothing in range | Firmware |
    |---|---|
    | **9999** | v3.0.0 or later |
    | Any other number | v2.0.0 |

=== "Any sensor, using a computer (v3.0.0 and later)"

    Connect the sensor to a computer by USB, open the Arduino IDE's **Serial Monitor** (any baud rate), and it prints:

    ```
    Advanced Bot Works IR Sensor Ring
    Firmware v3.0.0 (AdvancedBotWorks library), hardware v1.0
    ```

    If nothing appears, close and reopen the Serial Monitor. If it still prints nothing, the sensor is on v2.0.0 or earlier.

## What changed in 3.0.0

| | v2.0.0 | v3.0.0 |
|---|---|---|
| **IR Ring: no ball** | Red 0, blue 0 | Red 0, **blue 450**, intensity 0 |
| **IR Ring: strength (green)** | Strongest single receiver | Average of the strongest receiver and its two neighbours |
| **IR Ring: advanced direction** | Neighbour interpolation | Vector sum of nearby receivers (smoother) |
| **IR Ring: 4th value (intensity)** | Not used | **1 = ball detected** |
| **IR Ring: update rate** | Not fixed | 100 Hz, smoothed |
| **IR Ring: ball** | RCJA pulsed ball (previous) | RoboCup Junior IR golf ball |
| **Laser: distance mode** | Long | **Short** (about 1.3 m in any light) |
| **Laser: update rate** | About 20 Hz | 30 Hz |
| **Laser: nothing in range** | Raw reading | **9999** |
| **Laser: sensor fault** | Whole array stops | That channel shows **65535** |
| **Version reporting** | Always reported 1.0.0.0 to the hub | Real version to the hub, and printed over USB |

!!! tip "Updating your robot's code for v3.0.0"
    - **IR Ring:** if your code treats **blue = 0** as "no ball", change it to check **red = 0**, **blue = 450**, or the 4th value = 0.
    - **Laser:** handle **9999** (nothing in range) and **65535** (sensor fault), e.g. treat 9999 as "no wall".

The full list of changes is in the [changelog](../changelog.md).

## Updating or going back

You can install any version on your sensor yourself over USB. See [Updating firmware](updating.md). If you'd rather stay on an older version, that's fine: the product pages keep documenting both.
