# Laser Distance Sensor Array (v1)

Three laser time-of-flight distance sensors, facing **left, forward and right** at 90° to each other. It gives your robot millimetre distance readings to walls and obstacles, for example in RoboCup Junior Rescue Maze or for positioning on a Soccer field.

!!! info "At a glance"
    - **Measures:** distance to the nearest surface, in three directions
    - **Connects to:** LEGO® SPIKE™ Prime hub, any port (shows up as a colour sensor)
    - **Firmware:** `LaserArrayFirmware` from the [AdvancedBotWorks library](../firmware/library.md)
    - **Shop:** [advancedbotworks.com](https://advancedbotworks.com)

## Channel outputs

| Channel | Meaning | Range | Units | Notes |
|---|---|---|---|---|
| Red (R) | Left distance | 0 – about 4000 | mm | |
| Green (G) | Front distance | 0 – about 4000 | mm | |
| Blue (B) | Right distance | 0 – about 4000 | mm | |

!!! note "Accuracy beyond about 1.3 m"
    Readings are most accurate up to about **1.3 m**. The sensors can report distances of 3–4 m, but beyond about 1.3 m (and in bright ambient light) treat those readings as approximate.

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
            if front < 100:
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
| Current draw | TBC mA |
| Microcontroller | Microchip ATSAMD21G18A (ARM Cortex-M0+, 48 MHz) |
| Distance sensors | 3 × ST VL53L1X time-of-flight laser sensors (Class 1 laser, eye-safe) |
| USB | USB port on the board for firmware updates (see [Updating firmware](../firmware/updating.md)) |

### Performance

| | |
|---|---|
| Range | Most accurate up to about 1.3 m. Readings up to about 3–4 m are possible, but less accurate |
| Resolution | 1 mm |
| Accuracy | TBC (to be measured) |
| Update rate | About 20 Hz per sensor (TBC). A fixed, documented rate is planned |
| Surfaces | Works on most opaque surfaces. Very dark, shiny or transparent surfaces can give poor readings |

## In the box

- 1 × Laser Distance Sensor Array with attached SPIKE Prime cable

## Mounting

The array fits the LEGO® stud/pin grid. Mount it so that nothing on the robot blocks the three sensor windows, and keep the windows clean.

*Mounting diagram coming soon.*

## Firmware

- **Runs:** `LaserArrayFirmware` from the [AdvancedBotWorks library](../firmware/library.md), which uses Pololu's VL53L1X library
- **Source:** [github.com/advancedbotworks/abw-sensors](https://github.com/advancedbotworks/abw-sensors), open source under the GPL-3.0. Third-party licences are listed under [Licences](../licences.md)
- **Updates:** [Updating firmware](../firmware/updating.md)

## Known limitations

- **Bright sunlight and other infrared light** reduce the maximum range and accuracy.
- **Glass, mirrors and very dark matte surfaces** may give wrong or no readings.
- **Each sensor measures within a cone, not a single point,** so a surface at an angle or a narrow object can affect the reading.

## Revision history

| Version | Date | Changes |
|---|---|---|
| v1 | 2025-03 | First release (firmware library 2.0.0) |

!!! warning "Still to confirm"
    - Dimensions, weight, hole spacing and cable length
    - Current draw
    - Measured accuracy at several distances
    - Measured update rate (the firmware requests about 30 Hz, but the sensor's default timing gives about 20 Hz)
    - What's reported when no surface is in range or a reading fails
