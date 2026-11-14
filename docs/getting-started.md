# Getting started

All ABW sensors work the same way: plug one into the hub, and it appears as a **SPIKE colour sensor**. The sensor's data is sent on the colour sensor's **red (R), green (G) and blue (B)** channels. What each channel means depends on the product. See each product page for its channel table.

## What you need

- A LEGO® SPIKE™ Prime hub
- The SPIKE app (LEGO Education SPIKE App 3, in a browser or installed)
- Your ABW sensor. Its cable comes attached and plugs straight into any hub port (A–F).

## 1. Connect the sensor

1. Plug the sensor's cable into any port on the hub.
2. Turn the hub on and connect it to the SPIKE app.
3. The sensor shows up as a **Color Sensor** on that port.

!!! tip
    In the app's hub/device view, open the drop-down next to the sensor and choose the **RGB** view. The live red, green and blue values are your sensor's data.

## 2. Read the values in Word Blocks

The standard colour sensor blocks only give a colour or a reflection value. To read the raw red, green and blue values:

1. Open the **Extensions** menu (bottom-left of the block palette).
2. Add **More Sensors**.
3. Use the **Color Sensor red / green / blue** reporter blocks from the More Sensors extension, set to your port.

## 3. Read the values in Python

=== "SPIKE App 3"

    ```python
    from hub import port
    import color_sensor

    # r, g, b are the sensor's three data channels; i is a 4th value
    # (IR Ring, firmware v3.0.0: 1 = ball detected; otherwise 0)
    r, g, b, i = color_sensor.rgbi(port.A)
    print(r, g, b)
    ```

=== "Pybricks (advanced)"

    Pybricks' `ColorSensor` class doesn't expose the raw RGB channels. Read the sensor's RGB mode (mode 5) directly instead:

    ```python
    from pybricks.iodevices import PUPDevice
    from pybricks.parameters import Port

    sensor = PUPDevice(Port.A)
    r, g, b, i = sensor.read(5)
    print(r, g, b)
    ```

    !!! note
        This method hasn't been tested on every Pybricks version yet. Let us know how it goes.

Then see your product's page for what R, G and B mean. They can differ between [firmware versions](firmware/versions.md):

- [IR Sensor Ring](products/ir-ring-v1.md)
- [Laser Distance Sensor Array](products/laser-array-v1.md)

## Good to know

- **Readings start straight away.** No calibration is needed.
- **The sensor gets its power from the hub port.** There's no battery and no switch.
- **Treat the sensor like the electronics it is.** ABW sensors are not toys and are rated for ages 14+. Avoid pulling on the cable, and keep the board away from metal parts that could short it.
