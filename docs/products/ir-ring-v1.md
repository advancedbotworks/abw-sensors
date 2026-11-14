# IR Sensor Ring (v1)

A ring of 12 infrared receivers that tells your robot **which direction the RoboCup Junior Soccer IR ball is in, and how strong its signal is**, all the way around the robot.

!!! info "At a glance"
    - **Measures:** IR ball direction (12 sectors, or 0–360°) and signal strength
    - **Connects to:** LEGO® SPIKE™ Prime hub, any port (shows up as a colour sensor)
    - **Firmware:** `IRRingFirmware` from the [AdvancedBotWorks library](../firmware/library.md)
    - **Shop:** [advancedbotworks.com](https://advancedbotworks.com)

## Channel outputs

What the channels mean depends on the **firmware version** on your sensor. Sensors sold before **TBC (v3.0.0 release date)** run v2.0.0 unless they've been updated. See [Firmware versions](../firmware/versions.md) to check which one you have.

=== "Firmware v3.0.0 (current)"

    | Channel | Meaning | Range | Notes |
    |---|---|---|---|
    | Red (R) | Simple direction | 0–12 | Which receiver the ball is nearest (numbered 1–12 around the ring, see [Mounting](#mounting)). **0 = no ball** |
    | Green (G) | Signal strength | 0–100 | Average of the strongest receiver and its two neighbours. Higher usually means closer. 100 = ball point-blank |
    | Blue (B) | Advanced direction | 0–359° | Smooth angle from a vector sum of the receivers nearest the ball. **450 = no ball** |
    | Intensity (I) | Ball detected | 0 or 1 | **1 = ball detected, 0 = no ball.** Read it as the 4th value of `rgbi()` in Python |

    Values update **100 times per second** and are smoothed, so they're steady but still quick to react.

=== "Firmware v2.0.0"

    | Channel | Meaning | Range | Notes |
    |---|---|---|---|
    | Red (R) | Simple direction | 0–12 | Which receiver sees the strongest signal. **0 = no ball** |
    | Green (G) | Signal strength | 0–100 | Share of 100 quick samples in which the strongest receiver saw the ball |
    | Blue (B) | Advanced direction | 0–360° | Angle interpolated from the neighbouring receivers. **0 when no ball is detected** (also a valid angle, so check red or green as well) |
    | Intensity (I) | Not used | 0 | |

!!! note "Simple vs advanced direction"
    **Simple direction** (R) jumps in 30° steps, one per receiver, and is the easiest to use for a basic "chase the ball" program. **Advanced direction** (B) gives a smoother angle for more precise steering.

    The 12 receivers are spaced 30° apart, so sector *n* in simple direction corresponds to roughly (*n* − 1) × 30° in advanced direction. Check which way receiver 1 faces on your sensor before mounting it (see [Mounting](#mounting)).

## Quick start

1. Mount the ring flat and centred on your robot, with receiver 1 facing your robot's front.
2. Plug the cable into any hub port.
3. Read R, G and B (see [Getting started](../getting-started.md)). Move the ball around the robot and watch the direction change.

## Example code

=== "Word Blocks"

    Add the **More Sensors** extension, then use the **Color Sensor red** block for the simple direction, **green** for strength and **blue** for the advanced direction.

    To check whether the ball is visible, use **red = 0** (works on every firmware version). On v3.0.0 you can also check **blue = 450**.

=== "Python (firmware v3.0.0)"

    ```python
    from hub import port
    import color_sensor, runloop

    async def main():
        while True:
            direction, strength, angle, ball = color_sensor.rgbi(port.A)
            if not ball:                 # or: angle == 450
                print("No ball")
            else:
                print("Ball in sector", direction, "at", angle, "deg, strength", strength)
            await runloop.sleep_ms(10)   # new values every 10 ms

    runloop.run(main())
    ```

=== "Python (firmware v2.0.0)"

    ```python
    from hub import port
    import color_sensor, runloop

    async def main():
        while True:
            direction, strength, angle, _ = color_sensor.rgbi(port.A)
            if direction == 0:           # on v2, angle is also 0 when there's no ball
                print("No ball")
            else:
                print("Ball in sector", direction, "at", angle, "deg, strength", strength)
            await runloop.sleep_ms(50)

    runloop.run(main())
    ```

## Specifications

### Physical

| | |
|---|---|
| Dimensions | TBC mm |
| Weight | TBC g |
| Mounting | Fits the LEGO® stud/pin grid (hole spacing TBC) |
| Cable | Attached cable with SPIKE Prime connector, length TBC |

### Electrical

| | |
|---|---|
| Supply | From the SPIKE hub port |
| Current draw | TBC mA |
| Microcontroller | Microchip ATSAMD21G18A (ARM Cortex-M0+, 48 MHz) |
| IR receivers | 12 × Vishay TSSP77038 (38 kHz), 30° apart |
| USB | USB port on the board for firmware updates (see [Updating firmware](../firmware/updating.md)) |

### Performance

=== "Firmware v3.0.0 (current)"

    | | |
    |---|---|
    | Ball type | RoboCup Junior IR golf ball ([ir-golf-ball](https://github.com/robocup-junior/ir-golf-ball): 40 kHz carrier, 8 pulses per burst, 1.2 kHz burst rate) |
    | Sampling | All 12 receivers read together every 20 µs, over 10 ms windows |
    | Update rate | 100 Hz |
    | Smoothing | Exponential moving average (time constant about 20 ms) |
    | Direction | 12 sectors (simple); vector sum of nearby receivers (advanced) |
    | Detection range | TBC (to be measured with the IR golf ball) |

=== "Firmware v2.0.0"

    | | |
    |---|---|
    | Ball type | Tuned for the RoboCup Junior Australia pulsed IR ball (previous ball) |
    | Sampling | 100 quick reads of each receiver per update |
    | Update rate | Not fixed (depends on loop timing) |
    | Direction | 12 sectors (simple); neighbour interpolation (advanced) |

## In the box

- 1 × IR Sensor Ring with attached SPIKE Prime cable

## Mounting

The ring fits the LEGO® stud/pin grid. Mount it level, away from motors and metal that could block the receivers, and above other parts of the robot so nothing shadows the ball.

*Mounting diagram coming soon.*

## Firmware

- **Runs:** `IRRingFirmware` from the [AdvancedBotWorks library](../firmware/library.md)
- **Which version do I have?** See [Firmware versions](../firmware/versions.md)
- **Source:** [github.com/advancedbotworks/abw-sensors](https://github.com/advancedbotworks/abw-sensors), open source under the GPL-3.0
- **Updates:** [Updating firmware](../firmware/updating.md)

## Known limitations

- **Other 38 kHz IR sources** (some remote controls, other robots' IR, strong sunlight) can interfere with readings.
- **Strength isn't a calibrated distance.** It depends on the ball, its battery level and the environment.
- **Firmware v2.0.0 only:** the update rate isn't fixed, and blue = 0 can mean either "no ball" or "ball at 0°". Updating to v3.0.0 fixes both.

## Revision history

| Version | Date | Changes |
|---|---|---|
| v1 | 2024-06 | First release (firmware library 1.0.0) |
| v1 | TBC | Firmware v3.0.0: timer-based sampling at 100 Hz, IR golf ball, 450 = no ball, ball-detected flag |

!!! warning "Still to confirm"
    - Dimensions, weight, hole spacing and cable length
    - Current draw
    - Detection range with the IR golf ball (firmware v3.0.0)
    - Which way receiver 1 faces, and whether numbering runs clockwise (mounting diagram)
    - Firmware v3.0.0 release date (channel outputs note)
