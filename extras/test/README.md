# Host unit tests

Tests for the library's maths that run on a normal computer, with no Arduino or sensor needed. GitHub Actions runs them on every push and pull request.

```sh
make -C extras/test
```

Requires `g++` (or set `CXX`) and `make`.

## What's tested

`test_irring_math.cpp` covers the IR Sensor Ring maths in `src/sensors/IRRingMath.cpp`. It uses a simulated ball (a cos^k response per receiver):

- **Raw count to strength:** normalisation by the duty ceiling, clamping, bad-input guards
- **Smoothing (EMA)**
- **No ball:** 450 / 0 / not detected, below-threshold behaviour
- **Direction:**
    - a ball on each receiver gives its sector and angle (both methods)
    - accuracy all the way round
    - no jumps between receivers or at 0°/360°
    - angles stay in 0–359
    - fallback when vectors cancel
    - far-side reflections, with a limited span vs the whole ring
    - noise tolerance
- **Strength:**
    - the three-receiver average (including wrap-around at receiver 0)
    - less ripple than the single maximum
    - scaling and clamping
    - rises as the ball gets closer

The hardware parts (timer interrupt, GPIO, hub UART) can't run here. Test them on a real sensor with `ABW_IR_DEBUG`.
