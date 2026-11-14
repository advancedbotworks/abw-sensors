# FAQ and troubleshooting

Can't find your answer here? Ask in [GitHub Discussions (Q&A)](https://github.com/advancedbotworks/abw-sensors/discussions/categories/q-a), or see [Support](support.md) for anything about an order.

## Connecting a sensor

??? question "My sensor doesn't show up on the hub"
    1. Check the cable is pushed **all the way** into the hub port, then try a different port.
    2. Make sure the sensor **isn't also connected to a computer by USB**.
    3. Turn the hub off and on again, then reconnect it to the SPIKE app.
    4. Look in the app's hub/device view: the sensor should appear as a **Color Sensor** on its port.

    If it still doesn't appear on any port, [contact support](support.md).

??? question "Why does my ABW sensor show up as a colour sensor?"
    That's normal, and it's how all ABW sensors work. The hub sees each sensor as a LEGO® colour sensor, and the sensor sends its data on the **red, green and blue** channels. That means you can use it from Word Blocks or Python with no extra software. Your product's page explains what each channel means.

??? question "I can't find blocks for the red, green and blue values in Word Blocks"
    The standard colour sensor blocks don't include them. Open the **Extensions** menu (bottom-left of the block palette), add **More Sensors**, and use the **Color Sensor red / green / blue** blocks. See [Getting started](getting-started.md#2-read-the-values-in-word-blocks).

??? question "Can I use more than one ABW sensor on the same hub?"
    Yes. Each sensor uses its own port, so you can connect, for example, an IR Sensor Ring and a Laser Array to the same hub and read them separately.

??? question "Does it work with EV3, NXT or other hubs?"
    ABW **sensors** are designed and tested for the **LEGO® SPIKE™ Prime** hub. They don't work with EV3 or NXT, which use a different connector. Other hubs with the same connector haven't been tested.

    ABW **omni-wheels** fit any standard LEGO® cross axle, so they work on SPIKE, EV3 and NXT robots. See [Omni-Wheels and Rollers](products/omni-wheels.md).

??? question "Can I use Pybricks?"
    Yes, with a workaround: read the sensor's raw RGB mode directly. See the Pybricks tab in [Getting started](getting-started.md#3-read-the-values-in-python). This hasn't been tested on every Pybricks version yet, so please tell us how it goes.

## IR Sensor Ring

??? question "All the values are 0"
    A direction of 0 means **no ball is detected**. Check that:

    - the ball is switched on and its batteries aren't flat
    - you're using a **pulsed IR ball** of the kind used in RoboCup Junior Australia (see the next question)
    - nothing on the robot blocks the ring's view of the ball

??? question "Which ball does it work with?"
    The ring is tuned for the **pulsed IR ball used in RoboCup Junior Australia**. Testing with the ball chosen for 2027 is under way, and the results will appear on the [IR Sensor Ring](products/ir-ring-v1.md) page.

??? question "The direction jumps around or points the wrong way"
    - **Other infrared light** affects readings: sunlight, some remote controls and other robots' IR. Test away from windows.
    - **Check the ring's orientation.** Receiver 1 should face your robot's front. See [Mounting](products/ir-ring-v1.md#mounting).
    - For smoother steering, use the **advanced direction** (blue channel) instead of the simple direction (red).

??? question "Can I use the strength value as a distance?"
    Only roughly. Strength usually goes up as the ball gets closer, but it also depends on the ball, its battery level and the room. It's good for "is the ball close?", not for exact distances.

## Laser Distance Sensor Array

??? question "Can I use it in Soccer?"
    **No, not in RoboCup Junior Australia Standard or Lightweight Soccer.** The sensor emits infrared light, which isn't permitted in those leagues. It's intended mainly for **Rescue Maze**. Rules change every year, so always check the current rules for your league.

??? question "The readings are wrong, jumpy or very large"
    - **Bright light**, especially sunlight, greatly reduces the range (to under 1 m in strong light).
    - **Glass, mirrors and very dark or shiny surfaces** can give wrong readings or none at all.
    - **Below 4 cm** a surface is detected, but the distance isn't accurate.
    - Each sensor measures in a **27° cone**, not a single point, so angled walls and narrow objects can affect it.
    - Keep the **sensor windows clean** and make sure no part of the robot is in front of them.

## Firmware

??? question "Do I need to update the firmware?"
    Usually not. Sensors come with working firmware installed. Update only if a new release fixes something you need, or if we ask you to. See [Updating firmware](firmware/updating.md).

??? question "Which firmware version is on my sensor?"
    Every sensor sold so far runs **firmware library v2.0.0**. The [changelog](changelog.md) lists what each version changes.

??? question "Will updating the firmware void my warranty?"
    No. Installing official ABW firmware, or your own build of the open-source code, over USB doesn't affect your warranty. If something goes wrong, reinstall the official firmware using [Updating firmware](firmware/updating.md).

??? question "Can I change the firmware myself?"
    Yes. The firmware is open source under the GPL-3.0. See [Library for developers](firmware/library.md) and the [GitHub repository](https://github.com/advancedbotworks/abw-sensors).

## Omni-wheels

??? question "Do the replacement rollers fit both wheel sizes?"
    Yes. The 80 mm and 56 mm wheels use the same rollers. One 80-pack fully re-rolls four 80 mm wheels or five 56 mm wheels. See [Replacing rollers](products/omni-wheels.md#replacing-rollers).

??? question "My robot won't drive straight with omni-wheels"
    Omni-wheels roll freely sideways, so you need at least three driven wheels at angles to each other, and your code sets each motor's speed to choose the direction. Worn rollers on one wheel can also make the robot drift, so check them and replace them as a set if needed.

## Orders and warranty

??? question "When will my order ship?"
    Orders are sent **every Tuesday from Brisbane** with Australia Post. Pre-order items ship within the lead time shown on the product page.

??? question "Something's wrong with my product. What do I do?"
    Work through the questions above first. If that doesn't fix it, email **[support@advancedbotworks.com](mailto:support@advancedbotworks.com)** with your order number, the product, what's happening and a photo or video if you can. See [Support](support.md). Please don't post order details publicly on GitHub.
