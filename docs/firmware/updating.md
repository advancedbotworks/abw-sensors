# Updating firmware

Most teams never need to update their sensor's firmware. Update it only if a new firmware release fixes something you need, or if Advanced Bot Works support asks you to.

Each ABW sensor has a **USB port** and can be updated with the free **Arduino IDE**. The sensor's microcontroller is the same chip used on the Arduino Nano 33 IoT, so you upload firmware to it as if it were that board.

!!! warning "Before you start"
    - **Unplug the sensor from the SPIKE hub** before connecting it to your computer by USB.
    - Upload **only the firmware for your product**. Uploading the IR Ring firmware to a Laser Array (or the other way round) won't damage it, but the sensor won't work until you upload the right firmware.

## 1. Install the Arduino IDE

Download and install the **Arduino IDE 2.3.4 or later** from [arduino.cc/en/software](https://www.arduino.cc/en/software).

## 2. Install the board support

1. Open **Tools → Board → Boards Manager**.
2. Search for **Arduino SAMD Boards** and click **Install**.

## 3. Install the library and upload

Choose the firmware version you want. **v3.0.0** is the latest; **v2.0.0** is what sensors sold before TBC shipped with. Not sure which you have? See [Firmware versions](versions.md). If you're switching from v2 to v3, check the tip there about updating your robot's code.

=== "v3.0.0 (latest)"

    **Install**

    1. Open **Sketch → Include Library → Manage Libraries**, search for **VL53L1X** by **Pololu** and click **Install**. Only the Laser Array needs it, but it's harmless to install anyway.
    2. Download **Source code (zip)** from the [v3.0.0 release](https://github.com/advancedbotworks/abw-sensors/releases/tag/v3.0.0).
    3. Choose **Sketch → Include Library → Add .ZIP Library…** and select the ZIP.

    !!! warning "Already installed v2.0.0?"
        Remove the old **Advanced_Bot_Works_Sensors** folder from your Arduino `libraries` folder first. It bundles its own copy of VL53L1X, which can clash with Pololu's.

    **Upload**

    1. Connect the sensor to your computer with a USB cable.
    2. Choose **Tools → Board → Arduino SAMD Boards → Arduino NANO 33 IoT**.
    3. Choose the sensor's port under **Tools → Port**.
    4. Open your product's firmware from **File → Examples → AdvancedBotWorks**:
        - IR Sensor Ring: **IRRingFirmware**
        - Laser Distance Sensor Array: **LaserArrayFirmware**
    5. Click **Upload** (the right-arrow button) and wait for "Done uploading".
    6. Unplug the USB cable, plug the sensor back into the hub, and check the readings. On v3, the IR Ring shows **blue = 450** with no ball in view.

=== "v2.0.0"

    **Install**

    1. Download **Source code (zip)** from the [v2.0.0 release](https://github.com/advancedbotworks/abw-sensors/releases/tag/v2.0.0).
    2. Choose **Sketch → Include Library → Add .ZIP Library…** and select the ZIP. v2.0.0 includes everything it needs.

    !!! warning
        If you've installed v3.0.0 or Pololu's **VL53L1X** library, remove them from your Arduino `libraries` folder first. The two VL53L1X copies can clash and stop the Laser Array firmware compiling.

    **Upload**

    1. Connect the sensor to your computer with a USB cable.
    2. Choose **Tools → Board → Arduino SAMD Boards → Arduino NANO 33 IoT**.
    3. Choose the sensor's port under **Tools → Port**.
    4. Open your product's firmware from **File → Examples → Advanced_Bot_Works_Sensors**:
        - IR Sensor Ring: **IR_Sensor_Ring**
        - Laser Distance Sensor Array: **Laser_Distance_Sensor_Array**
    5. Click **Upload** and wait for "Done uploading".
    6. Unplug the USB cable, plug the sensor back into the hub, and check the readings.

## Troubleshooting

| Problem | Try |
|---|---|
| The sensor doesn't appear under **Tools → Port** | Try another USB cable (some cables only charge and don't carry data) and another USB port on your computer. |
| The upload fails partway through | Unplug and replug the USB cable, then upload again. |
| The hub no longer recognises the sensor after an update | Check that you uploaded the right firmware for your product, then contact [support](../support.md). |
| "multiple definition" or VL53L1X errors when compiling | Two copies of the VL53L1X driver are installed. Follow the warning in your version's tab above. |
| The robot behaves differently after updating to v3 | Some channel values changed (e.g. IR blue = 450 for no ball). See [Firmware versions](versions.md). |
