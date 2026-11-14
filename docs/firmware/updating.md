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
2. Search for **Arduino SAMD Boards (32-bits ARM Cortex-M0+)** and click **Install**.

## 3. Install the ABW library

=== "Library Manager (once published)"

    1. Open **Sketch → Include Library → Manage Libraries**.
    2. Search for **AdvancedBotWorks** and click **Install**.
    3. When asked, click **Install All** so that its dependency (Pololu's **VL53L1X** library) is installed too.

=== "From a ZIP file"

    1. Download the latest release ZIP from [github.com/advancedbotworks/abw-sensors/releases](https://github.com/advancedbotworks/abw-sensors/releases).
    2. In the Arduino IDE, choose **Sketch → Include Library → Add .ZIP Library…** and select the ZIP.
    3. **Laser Array only:** also install **VL53L1X** by Pololu from **Sketch → Include Library → Manage Libraries**.

## 4. Upload the firmware

1. Connect the sensor to your computer with a USB cable.
2. Choose **Tools → Board → Arduino SAMD Boards → Arduino NANO 33 IoT**.
3. Choose the sensor's port under **Tools → Port**.
4. Open your product's firmware from **File → Examples → AdvancedBotWorks**:
    - IR Sensor Ring: **IRRingFirmware**
    - Laser Distance Sensor Array: **LaserArrayFirmware**
5. Click **Upload** (the right-arrow button) and wait for "Done uploading".
6. Unplug the USB cable, plug the sensor back into the hub, and check the readings.

## Troubleshooting

| Problem | Try |
|---|---|
| The sensor doesn't appear under **Tools → Port** | Try another USB cable (some cables only charge and don't carry data) and another USB port on your computer. |
| The upload fails partway through | Unplug and replug the USB cable, then upload again. |
| The hub no longer recognises the sensor after an update | Check that you uploaded the right firmware for your product, then contact [support](../support.md). |
