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

## 3. Install the ABW library

!!! info "Current release: v2.0.0"
    These steps are for **firmware library v2.0.0**, the version installed on all sensors sold so far. The [changelog](../changelog.md) lists what each version changes.

1. Go to [github.com/advancedbotworks/abw-sensors/releases/latest](https://github.com/advancedbotworks/abw-sensors/releases/latest) and download **Source code (zip)**.
2. In the Arduino IDE, choose **Sketch → Include Library → Add .ZIP Library…** and select the ZIP you downloaded.
3. You don't need any other libraries. v2.0.0 includes everything it needs.

!!! warning
    If you've installed Pololu's **VL53L1X** library separately, the Arduino IDE can mix up the two copies and the Laser Array firmware may fail to compile. If that happens, remove the separate VL53L1X library from your Arduino `libraries` folder and try again.

## 4. Upload the firmware

1. Connect the sensor to your computer with a USB cable.
2. Choose **Tools → Board → Arduino SAMD Boards → Arduino NANO 33 IoT**.
3. Choose the sensor's port under **Tools → Port**.
4. Open your product's firmware from **File → Examples → Advanced_Bot_Works_Sensors**:
    - IR Sensor Ring: **IR_Sensor_Ring**
    - Laser Distance Sensor Array: **Laser_Distance_Sensor_Array**
5. Click **Upload** (the right-arrow button) and wait for "Done uploading".
6. Unplug the USB cable, plug the sensor back into the hub, and check the readings.

## Troubleshooting

| Problem | Try |
|---|---|
| The sensor doesn't appear under **Tools → Port** | Try another USB cable (some cables only charge and don't carry data) and another USB port on your computer. |
| The upload fails partway through | Unplug and replug the USB cable, then upload again. |
| The hub no longer recognises the sensor after an update | Check that you uploaded the right firmware for your product, then contact [support](../support.md). |
