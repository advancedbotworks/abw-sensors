# IR Sensor Ring (v1)

A ring of 12 infrared receivers that tells your robot **which direction the RoboCup Junior Soccer ball is in, and how strong its signal is**, all the way around the robot.

!!! info "At a glance"
    - **Measures:** IR ball direction (12 sectors, or 0–360°) and signal strength
    - **Connects to:** LEGO® SPIKE™ Prime hub, any port (shows up as a colour sensor)
    - **Firmware:** `IRRingFirmware` from the [AdvancedBotWorks library](../firmware/library.md)
    - **Shop:** [advancedbotworks.com](https://advancedbotworks.com)

## Channel outputs

| Channel | Meaning | Range | Units | Notes |
|---|---|---|---|---|
| Red (R) | Simple direction | 0–12 | sector | Which of the 12 receivers sees the strongest signal (numbered 1–12 around the ring, see [Mounting](#mounting)). **0 = no ball detected** |
| Green (G) | Signal strength | 0–100 | – | The share of samples in which the strongest receiver saw the ball. Higher usually means closer |
| Blue (B) | Advanced direction | 0–360 | degrees | Smoothed angle, interpolated from the neighbouring receivers. **0 when no ball is detected** |

!!! note "Simple vs advanced direction"
    **Simple direction** (R) jumps in 30° steps, one per receiver, and is the easiest to use for a basic "chase the ball" program. **Advanced direction** (B) blends the readings of the receivers either side of the strongest one, giving a smoother angle for more precise steering.

    The 12 receivers are spaced 30° apart, so sector *n* in simple direction corresponds to roughly *n* × 30° in advanced direction. Check which way receiver 1 faces on your sensor before mounting it (see [Mounting](#mounting)).

## Quick start

1. Mount the ring flat and centred on your robot, with receiver 1 facing your robot's front.
2. Plug the cable into any hub port.
3. Read R, G and B (see [Getting started](../getting-started.md)). Move the ball around the robot and watch the direction change.

## Example code

=== "Word Blocks"

    Add the **More Sensors** extension, then use the **Color Sensor red** block for the simple direction, **green** for strength and **blue** for the advanced direction.

=== "SPIKE App 3 Python"

    ```python
    from hub import port
    import color_sensor, runloop

    async def main():
        while True:
            direction, strength, angle, _ = color_sensor.rgbi(port.A)
            if direction == 0:
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
| Microcontroller | 32-bit ARM microcontroller |
| IR receivers | 12 × 38 kHz IR receivers, 30° apart |
| USB | USB port on the board for firmware updates (see [Updating firmware](../firmware/updating.md)) |

### Performance

| | |
|---|---|
| Ball type | Tuned for the RoboCup Junior Australia pulsed IR ball. Testing with the ball chosen for 2027 is in progress |
| Direction resolution | 12 sectors (simple) / interpolated angle (advanced) |
| Update rate | Not yet fixed. Each update takes 100 samples of all 12 receivers, so the rate depends on loop timing. A fixed, documented rate is planned |
| Detection range | TBC (to be measured with the RCJA and 2027 balls) |

## In the box

- 1 × IR Sensor Ring with attached SPIKE Prime cable

## Mounting

The ring fits the LEGO® stud/pin grid. Mount it level, away from motors and metal that could block the receivers, and above other parts of the robot so nothing shadows the ball.

*Mounting diagram coming soon.*

## Firmware

- **Runs:** `IRRingFirmware` from the [AdvancedBotWorks library](../firmware/library.md)
- **Source:** [github.com/advancedbotworks/abw-sensors](https://github.com/advancedbotworks/abw-sensors), open source under the GPL-3.0
- **Updates:** [Updating firmware](../firmware/updating.md)

## Known limitations

- **Other 38 kHz IR sources** (some remote controls, other robots' IR, strong sunlight) can interfere with readings.
- **Strength isn't a calibrated distance.** It depends on the ball, its battery level and the environment.
- **The update rate isn't fixed yet** (see Performance above).

## Revision history

| Version | Date | Changes |
|---|---|---|
| v1 | 2024-06 | First release (firmware library 1.0.0) |

!!! warning "Still to confirm"
    - Dimensions, weight, hole spacing and cable length
    - Current draw
    - Detection range (RCJA ball and 2027 ball)
    - Which way receiver 1 faces, and whether numbering runs clockwise (mounting diagram)
    - Measured update rate
