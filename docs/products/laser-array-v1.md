# Laser Distance Sensor Array (v1)

Three laser time-of-flight distance sensors, facing **left, forward and right** at 90° to each other. It gives your robot millimetre distance readings to walls and obstacles, for example for wall-following in RoboCup Junior Rescue Maze.

!!! danger "Not permitted in RoboCup Junior Australia Soccer"
    This sensor **emits infrared light**, so it is **not permitted in RoboCup Junior Australia Standard or Lightweight Soccer**. It's intended mainly for **Rescue Maze**. Rules change each year: always check the current rules for your league before using any sensor in competition.

!!! info "At a glance"
    - **Measures:** distance to the nearest surface, in three directions
    - **Connects to:** LEGO® SPIKE™ Prime hub, any port (shows up as a colour sensor)
    - **Firmware:** `LaserArrayFirmware` from the [AdvancedBotWorks library](../firmware/library.md)
    - **Shop:** [advancedbotworks.com](https://advancedbotworks.com)

## Channel outputs

What the channels mean depends on the **firmware version** on your sensor. See [Firmware versions](../firmware/versions.md) to check which one you have.

=== "Firmware v3.0.0 (current)"

    | Channel | Meaning | Units |
    |---|---|---|
    | Red (R) | Left distance | mm |
    | Green (G) | Front distance | mm |
    | Blue (B) | Right distance | mm |
    | Intensity (I) | Not used (0) | |

    **Special values** (instead of a distance):

    | Value | Meaning |
    |---|---|
    | **9999** | Nothing within range (or the reading wasn't valid) |
    | **65535** | That sensor isn't working (failed to start or stopped responding). The other two keep working |

    Each sensor updates **30 times per second**.

=== "Firmware v2.0.0"

    | Channel | Meaning | Units |
    |---|---|---|
    | Red (R) | Left distance | mm |
    | Green (G) | Front distance | mm |
    | Blue (B) | Right distance | mm |
    | Intensity (I) | Not used (0) | |

    No special values: when nothing is in range, the sensor reports whatever reading it gets. If a sensor fails to start, the whole array stops responding.

!!! note "How far it can see depends on the mode, the light and the surface"
    **Firmware v3.0.0** uses the VL53L1X's **Short distance mode** by default: about **1.3 m in any lighting**, the most predictable choice for Rescue Maze. Advanced users can change the mode in the firmware (`ABW_LASER_DISTANCE_MODE`). **Firmware v2.0.0** uses **Long distance mode**.

    ST's datasheet figures (white target, 100 ms timing budget):

    | Mode | Dark | Bright ambient light |
    |---|---|---|
    | Short (v3 default) | 136 cm | 135 cm |
    | Medium | 290 cm | 76 cm |
    | Long (v2) | 360 cm | 73 cm |

    - **Ranging error:** ±20 mm in the dark, ±25 mm in ambient light.
    - **Below 4 cm,** a target is detected but the distance isn't accurate.
    - Darker targets reduce the range in Medium and Long modes (e.g. 170 cm for a dark grey target in Long mode, dark conditions).

## Quick start

1. Mount the array with the middle sensor facing your robot's front.
2. Plug the cable into any hub port.
3. Read R, G and B (see [Getting started](../getting-started.md)). Hold a book in front of each sensor and watch its value change.

## Example code

=== "Word Blocks"

    Add the **More Sensors** extension, then use **Color Sensor red** for the left distance, **green** for the front and **blue** for the right. All values are in millimetres.

=== "SPIKE App 3 Python"

    ```python
    from hub import port
    import color_sensor, runloop

    async def main():
        while True:
            left, front, right, _ = color_sensor.rgbi(port.A)
            print("L", left, "mm  F", front, "mm  R", right, "mm")
            if front == 9999:            # firmware v3.0.0: nothing in range
                print("No wall ahead")
            elif front < 100:
                print("Wall ahead!")
            await runloop.sleep_ms(50)

    runloop.run(main())
    ```

## Specifications

### Physical

| | |
|---|---|
| Dimensions | TBC mm |
| Weight | TBC g |
| Sensor directions | Left, front, right (90° apart) |
| Mounting | Fits the LEGO® stud/pin grid (hole spacing TBC) |
| Cable | Attached cable with SPIKE Prime connector, length TBC |

### Electrical

| | |
|---|---|
| Supply | From the SPIKE hub port |
| Current draw | TBC (measured). The three VL53L1X sensors alone draw about 48 mA typical while ranging (16 mA each, ST datasheet) |
| Microcontroller | Microchip ATSAMD21G18A (ARM Cortex-M0+, 48 MHz) |
| Distance sensors | 3 × ST VL53L1X time-of-flight laser sensors (940 nm, Class 1 laser, eye-safe) |
| USB | USB port on the board for firmware updates (see [Updating firmware](../firmware/updating.md)) |

### Performance

=== "Firmware v3.0.0 (current)"

    | | |
    |---|---|
    | Distance mode | Short (default), configurable in firmware |
    | Range (ST datasheet, Short mode) | About 1.3 m in any lighting |
    | Minimum distance | 4 cm |
    | Ranging error (ST datasheet) | ±20 mm (dark), ±25 mm (ambient light) |
    | Resolution | 1 mm |
    | Field of view | 27° cone per sensor |
    | Update rate | 30 Hz per sensor (33 ms timing budget) |

=== "Firmware v2.0.0"

    | | |
    |---|---|
    | Distance mode | Long |
    | Range (ST datasheet, Long mode) | Up to 360 cm in the dark (white target); about 70 cm in bright ambient light |
    | Minimum distance | 4 cm |
    | Ranging error (ST datasheet) | ±20 mm (dark), ±25 mm (ambient light) |
    | Resolution | 1 mm |
    | Field of view | 27° cone per sensor |
    | Update rate | About 20 Hz per sensor |

## In the box

- 1 × Laser Distance Sensor Array with attached SPIKE Prime cable

## Mounting

The array fits the LEGO® stud/pin grid. Mount it so that nothing on the robot blocks the three sensor windows, and keep the windows clean.

*Mounting diagram coming soon.*

## Firmware

- **Runs:** `LaserArrayFirmware` from the [AdvancedBotWorks library](../firmware/library.md), which uses Pololu's VL53L1X library
- **Which version do I have?** See [Firmware versions](../firmware/versions.md)
- **Source:** [github.com/advancedbotworks/abw-sensors](https://github.com/advancedbotworks/abw-sensors), open source under the GPL-3.0. Third-party licences are listed under [Licences](../licences.md)
- **Updates:** [Updating firmware](../firmware/updating.md)

## Known limitations

- **Bright sunlight and other infrared light** greatly reduce the maximum range in Medium and Long modes (to under 1 m in strong ambient light). Short mode (v3 default) is barely affected.
- **Glass, mirrors and very dark matte surfaces** may give wrong or no readings.
- **Each sensor measures within a 27° cone, not a single point,** so a surface at an angle or a narrow object can affect the reading.

## Revision history

| Version | Date | Changes |
|---|---|---|
| v1 | 2025-03 | First release (firmware library 2.0.0) |
| v1 | TBC | Firmware v3.0.0: Short mode default, 30 Hz, special values 9999 / 65535 |

!!! warning "Still to confirm"
    - Dimensions, weight, hole spacing and cable length
    - Current draw
    - Measured range and accuracy on ABW hardware (with the 50 ms timing budget, under competition lighting)
    - Measured update rate on hardware (v3.0.0 is designed for 30 Hz)
    - What's reported when no surface is in range or a reading fails
