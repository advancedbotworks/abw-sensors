/*
 * Advanced Bot Works Sensors - host unit tests for the IR Sensor Ring maths
 * Copyright (C) 2026 Aparaj Bogahawatta
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Runs on a normal computer (no Arduino, no hardware):
 *     make -C extras/test
 *
 * The ball is simulated: each receiver's smoothed strength follows a
 * cos^k lobe of the angle between the receiver and the ball, which is a
 * reasonable stand-in for an IR receiver's directional response.
 */
#include "IRRingMath.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <initializer_list>

using namespace abw;

// ---------------------------------------------------------------------------
// Minimal test framework
// ---------------------------------------------------------------------------
static int g_checks = 0;
static int g_failures = 0;
static const char* g_test = "";

#define CHECK(cond)                                                          \
    do {                                                                     \
        g_checks++;                                                          \
        if (!(cond)) {                                                       \
            g_failures++;                                                    \
            printf("  FAIL [%s] %s:%d: %s\n", g_test, __FILE__, __LINE__, #cond); \
        }                                                                    \
    } while (0)

#define CHECK_MSG(cond, ...)                                                 \
    do {                                                                     \
        g_checks++;                                                          \
        if (!(cond)) {                                                       \
            g_failures++;                                                    \
            printf("  FAIL [%s] %s:%d: %s -- ", g_test, __FILE__, __LINE__, #cond); \
            printf(__VA_ARGS__);                                             \
            printf("\n");                                                    \
        }                                                                    \
    } while (0)

#define TEST(name) static void name()
#define RUN(name)                                                            \
    do {                                                                     \
        g_test = #name;                                                      \
        int before = g_failures;                                             \
        name();                                                              \
        printf("%s %s\n", g_failures == before ? "PASS" : "FAIL", #name);    \
    } while (0)

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static const int N = ABW_IR_SENSOR_COUNT;

static IRRingSettings defaults(IRDirectionMethod method = IR_DIRECTION_VECTOR_SUM,
                               uint8_t span = 2) {
    IRRingSettings s;
    s.strengthMax = 100.0f;
    s.noBallThreshold = 3.0f;
    s.method = method;
    s.vectorSpan = span;
    return s;
}

// Simulated smoothed strengths for a ball at `ballDeg` degrees.
static void simulateBall(float out[], float ballDeg, float peak = 100.0f, float power = 2.0f) {
    for (int i = 0; i < N; i++) {
        float diff = (i * 30.0f - ballDeg) * (float)M_PI / 180.0f;
        float c = cosf(diff);
        out[i] = c > 0.0f ? peak * powf(c, power) : 0.0f;
    }
}

// Smallest difference between two angles, in degrees (0-180).
static float angleError(float a, float b) {
    float d = fmodf(fabsf(a - b), 360.0f);
    return d > 180.0f ? 360.0f - d : d;
}

// Deterministic pseudo-random noise in [-amp, amp].
static float noise(unsigned& seed, float amp) {
    seed = seed * 1103515245u + 12345u;
    return amp * (((seed >> 16) & 0x7FFF) / 16383.5f - 1.0f);
}

// ---------------------------------------------------------------------------
// Tests: raw count -> strength, smoothing
// ---------------------------------------------------------------------------
TEST(count_to_strength_normalises_by_duty) {
    // 500 samples, duty ceiling 0.24 -> 120 low samples = point-blank = 100.
    CHECK(fabsf(irCountToStrength(120, 500, 0.24f) - 100.0f) < 0.01f);
    CHECK(fabsf(irCountToStrength(60, 500, 0.24f) - 50.0f) < 0.01f);
    CHECK(irCountToStrength(0, 500, 0.24f) == 0.0f);
}

TEST(count_to_strength_clamps_and_guards) {
    CHECK(irCountToStrength(500, 500, 0.24f) == 100.0f);   // above the ceiling
    CHECK(irCountToStrength(100, 0, 0.24f) == 0.0f);       // no samples
    CHECK(irCountToStrength(100, 500, 0.0f) == 0.0f);      // bad calibration
}

TEST(smoothing_converges_and_alpha_one_passes_through) {
    CHECK(irSmooth(10.0f, 50.0f, 1.0f) == 50.0f);
    float v = 0.0f;
    for (int i = 0; i < 50; i++) v = irSmooth(v, 80.0f, 0.4f);
    CHECK(fabsf(v - 80.0f) < 0.01f);
    // One step at alpha 0.4 moves 40% of the way.
    CHECK(fabsf(irSmooth(0.0f, 100.0f, 0.4f) - 40.0f) < 0.001f);
}

// ---------------------------------------------------------------------------
// Tests: no ball
// ---------------------------------------------------------------------------
TEST(no_ball_when_all_zero) {
    float v[N] = {0};
    IRRingOutputs o = irCalculateOutputs(v, defaults());
    CHECK(!o.ballDetected);
    CHECK(o.directionSimple == ABW_IR_NO_BALL_DIRECTION);
    CHECK(o.directionAdvanced == ABW_IR_NO_BALL_ANGLE);
    CHECK(o.strength == 0);
}

TEST(no_ball_below_threshold) {
    float v[N];
    simulateBall(v, 90.0f, 2.5f);   // very weak: below the 3% threshold
    IRRingOutputs o = irCalculateOutputs(v, defaults());
    CHECK(!o.ballDetected);
    CHECK(o.directionAdvanced == 450);
    CHECK(o.directionSimple == 0);
}

TEST(no_ball_with_zero_threshold_and_no_signal) {
    float v[N] = {0};
    IRRingSettings s = defaults();
    s.noBallThreshold = 0.0f;
    CHECK(!irCalculateOutputs(v, s).ballDetected);
}

TEST(ball_detected_just_above_threshold) {
    float v[N];
    simulateBall(v, 90.0f, 10.0f);
    IRRingOutputs o = irCalculateOutputs(v, defaults());
    CHECK(o.ballDetected);
    CHECK(o.directionAdvanced != 450);
}

// ---------------------------------------------------------------------------
// Tests: direction
// ---------------------------------------------------------------------------
TEST(ball_on_each_receiver_gives_its_sector_and_angle) {
    const IRDirectionMethod methods[] = {IR_DIRECTION_VECTOR_SUM, IR_DIRECTION_INTERPOLATION};
    for (IRDirectionMethod m : methods) {
        for (int i = 0; i < N; i++) {
            float v[N];
            simulateBall(v, i * 30.0f);
            IRRingOutputs o = irCalculateOutputs(v, defaults(m));
            CHECK_MSG(o.directionSimple == i + 1, "method %d receiver %d -> sector %d", m, i, o.directionSimple);
            CHECK_MSG(angleError(o.directionAdvanced, i * 30.0f) <= 1.0f,
                      "method %d receiver %d -> %d deg", m, i, o.directionAdvanced);
        }
    }
}

static float worstSweepError(IRDirectionMethod method, uint8_t span, float power) {
    float worst = 0.0f;
    for (int deg = 0; deg < 360; deg++) {
        float v[N];
        simulateBall(v, (float)deg, 100.0f, power);
        IRRingOutputs o = irCalculateOutputs(v, defaults(method, span));
        float e = angleError(o.directionAdvanced, (float)deg);
        if (e > worst) worst = e;
    }
    return worst;
}

TEST(vector_sum_accuracy_all_round) {
    for (float power : {2.0f, 4.0f}) {
        float e2 = worstSweepError(IR_DIRECTION_VECTOR_SUM, 2, power);
        float e6 = worstSweepError(IR_DIRECTION_VECTOR_SUM, 6, power);
        CHECK_MSG(e2 <= 2.5f, "span 2, cos^%.0f: worst error %.2f deg", power, e2);
        CHECK_MSG(e6 <= 1.5f, "span 6, cos^%.0f: worst error %.2f deg", power, e6);
    }
}

TEST(interpolation_accuracy_all_round) {
    float e = worstSweepError(IR_DIRECTION_INTERPOLATION, 2, 2.0f);
    CHECK_MSG(e <= 6.0f, "worst error %.2f deg", e);
}

TEST(vector_sum_is_at_least_as_accurate_as_interpolation) {
    CHECK(worstSweepError(IR_DIRECTION_VECTOR_SUM, 2, 2.0f) <=
          worstSweepError(IR_DIRECTION_INTERPOLATION, 2, 2.0f));
}

TEST(direction_is_continuous_no_jumps) {
    // Moving the ball 1 degree should never move the reported angle by more
    // than a few degrees (no jumps at receiver boundaries or at 0/360).
    for (uint8_t span : {(uint8_t)2, (uint8_t)6}) {
        float v[N];
        simulateBall(v, 0.0f);
        uint16_t prev = irCalculateOutputs(v, defaults(IR_DIRECTION_VECTOR_SUM, span)).directionAdvanced;
        float worstStep = 0.0f;
        for (int deg = 1; deg <= 360; deg++) {
            simulateBall(v, (float)deg);
            uint16_t now = irCalculateOutputs(v, defaults(IR_DIRECTION_VECTOR_SUM, span)).directionAdvanced;
            float step = angleError(now, prev);
            if (step > worstStep) worstStep = step;
            prev = now;
        }
        CHECK_MSG(worstStep <= 3.0f, "span %d: biggest jump %.1f deg", span, worstStep);
    }
}

TEST(angle_always_in_range_never_360) {
    for (int tenth = 0; tenth < 3600; tenth++) {
        float v[N];
        simulateBall(v, tenth / 10.0f);
        for (IRDirectionMethod m : {IR_DIRECTION_VECTOR_SUM, IR_DIRECTION_INTERPOLATION}) {
            IRRingOutputs o = irCalculateOutputs(v, defaults(m));
            CHECK_MSG(o.directionAdvanced < 360, "ball %.1f deg -> %d", tenth / 10.0f, o.directionAdvanced);
            CHECK(o.directionSimple >= 1 && o.directionSimple <= 12);
        }
    }
}

TEST(wraps_correctly_around_zero) {
    float v[N];
    simulateBall(v, 355.0f);
    IRRingOutputs o = irCalculateOutputs(v, defaults());
    CHECK_MSG(angleError(o.directionAdvanced, 355.0f) <= 2.5f, "-> %d", o.directionAdvanced);
    simulateBall(v, 5.0f);
    o = irCalculateOutputs(v, defaults());
    CHECK_MSG(angleError(o.directionAdvanced, 5.0f) <= 2.5f, "-> %d", o.directionAdvanced);
}

TEST(position_to_degrees_wraps) {
    CHECK(irPositionToDegrees(0.0f) == 0);
    CHECK(irPositionToDegrees(3.0f) == 90);
    CHECK(irPositionToDegrees(-1.0f) == 330);
    CHECK(irPositionToDegrees(12.0f) == 0);
    CHECK(irPositionToDegrees(11.999f) == 0);   // rounds to 360 -> 0
    CHECK(irPositionToDegrees(25.0f) == 30);
}

TEST(tie_picks_first_receiver) {
    float v[N] = {0};
    v[3] = 50.0f;
    v[7] = 50.0f;
    CHECK(irStrongest(v) == 3);
}

TEST(cancelling_vectors_fall_back_to_strongest) {
    // Two opposite receivers equally bright: the whole-ring sum cancels.
    float v[N] = {0};
    v[2] = 60.0f;
    v[8] = 60.0f;
    IRRingOutputs o = irCalculateOutputs(v, defaults(IR_DIRECTION_VECTOR_SUM, 6));
    CHECK(o.ballDetected);
    CHECK_MSG(o.directionAdvanced == 60, "-> %d", o.directionAdvanced);   // receiver 2 = 60 deg
}

TEST(limited_span_ignores_far_side_reflection) {
    // Ball at 90 deg plus a reflection at 270 deg (opposite side).
    float v[N], reflection[N];
    simulateBall(v, 90.0f, 100.0f);
    simulateBall(reflection, 270.0f, 40.0f);
    for (int i = 0; i < N; i++) v[i] += reflection[i];

    float e2 = angleError(irCalculateOutputs(v, defaults(IR_DIRECTION_VECTOR_SUM, 2)).directionAdvanced, 90.0f);
    CHECK_MSG(e2 <= 1.0f, "span 2 error %.1f", e2);

    // Reflection at 150 deg (60 deg away): whole ring is pulled, span 2 less so.
    simulateBall(v, 90.0f, 100.0f);
    simulateBall(reflection, 210.0f, 50.0f);
    for (int i = 0; i < N; i++) v[i] += reflection[i];
    float span2 = angleError(irCalculateOutputs(v, defaults(IR_DIRECTION_VECTOR_SUM, 2)).directionAdvanced, 90.0f);
    float span6 = angleError(irCalculateOutputs(v, defaults(IR_DIRECTION_VECTOR_SUM, 6)).directionAdvanced, 90.0f);
    CHECK_MSG(span2 <= span6, "span2 %.1f vs span6 %.1f", span2, span6);
}

TEST(noise_keeps_angle_close) {
    unsigned seed = 1234;
    float worst = 0.0f;
    for (int trial = 0; trial < 2000; trial++) {
        float deg = (float)(trial % 360);
        float v[N];
        simulateBall(v, deg, 60.0f);
        for (int i = 0; i < N; i++) {
            v[i] += noise(seed, 2.0f);      // +/-2 strength units of noise
            if (v[i] < 0.0f) v[i] = 0.0f;
        }
        float e = angleError(irCalculateOutputs(v, defaults()).directionAdvanced, deg);
        if (e > worst) worst = e;
    }
    CHECK_MSG(worst <= 6.0f, "worst error with noise %.1f deg", worst);
}

// ---------------------------------------------------------------------------
// Tests: strength
// ---------------------------------------------------------------------------
TEST(strength_is_three_receiver_average) {
    float v[N] = {0};
    v[4] = 90.0f;
    v[3] = 30.0f;
    v[5] = 60.0f;
    v[10] = 80.0f;   // far-side value must not count
    CHECK(fabsf(irStrength(v, irStrongest(v), 100.0f) - 60.0f) < 0.01f);
    CHECK(irCalculateOutputs(v, defaults()).strength == 60);
}

TEST(strength_wraps_neighbours_at_receiver_zero) {
    float v[N] = {0};
    v[0] = 90.0f;
    v[11] = 30.0f;
    v[1] = 30.0f;
    CHECK(fabsf(irStrength(v, 0, 100.0f) - 50.0f) < 0.01f);
}

TEST(strength_ripple_smaller_than_single_max) {
    float minAvg = 1e9f, maxAvg = 0.0f, minMax = 1e9f, maxMax = 0.0f;
    for (int deg = 0; deg < 360; deg++) {
        float v[N];
        simulateBall(v, (float)deg, 100.0f, 4.0f);
        uint8_t best = irStrongest(v);
        float avg = irStrength(v, best, 100.0f);
        float mx = v[best];
        if (avg < minAvg) minAvg = avg;
        if (avg > maxAvg) maxAvg = avg;
        if (mx < minMax) minMax = mx;
        if (mx > maxMax) maxMax = mx;
    }
    float rippleAvg = (maxAvg - minAvg) / maxAvg;
    float rippleMax = (maxMax - minMax) / maxMax;
    CHECK_MSG(rippleAvg < rippleMax, "avg ripple %.1f%% vs max ripple %.1f%%", 100 * rippleAvg, 100 * rippleMax);
    CHECK_MSG(rippleAvg < 0.10f, "avg ripple %.1f%%", 100 * rippleAvg);
}

TEST(strength_scaling_and_clamp) {
    float v[N] = {0};
    v[6] = 60.0f;
    v[5] = 30.0f;
    v[7] = 30.0f;   // average 40
    CHECK(fabsf(irStrength(v, 6, 50.0f) - 80.0f) < 0.01f);   // 40 / 50 -> 80
    CHECK(irStrength(v, 6, 20.0f) == 100.0f);                 // clamped
    IRRingSettings s = defaults();
    s.strengthMax = 50.0f;
    CHECK(irCalculateOutputs(v, s).strength == 80);
}

TEST(strength_rises_as_ball_gets_closer) {
    uint8_t prev = 0;
    for (float peak = 10.0f; peak <= 100.0f; peak += 10.0f) {
        float v[N];
        simulateBall(v, 45.0f, peak);
        uint8_t s = irCalculateOutputs(v, defaults()).strength;
        CHECK_MSG(s >= prev, "peak %.0f -> %d (prev %d)", peak, s, prev);
        prev = s;
    }
}

// ---------------------------------------------------------------------------
int main() {
    RUN(count_to_strength_normalises_by_duty);
    RUN(count_to_strength_clamps_and_guards);
    RUN(smoothing_converges_and_alpha_one_passes_through);
    RUN(no_ball_when_all_zero);
    RUN(no_ball_below_threshold);
    RUN(no_ball_with_zero_threshold_and_no_signal);
    RUN(ball_detected_just_above_threshold);
    RUN(ball_on_each_receiver_gives_its_sector_and_angle);
    RUN(vector_sum_accuracy_all_round);
    RUN(interpolation_accuracy_all_round);
    RUN(vector_sum_is_at_least_as_accurate_as_interpolation);
    RUN(direction_is_continuous_no_jumps);
    RUN(angle_always_in_range_never_360);
    RUN(wraps_correctly_around_zero);
    RUN(position_to_degrees_wraps);
    RUN(tie_picks_first_receiver);
    RUN(cancelling_vectors_fall_back_to_strongest);
    RUN(limited_span_ignores_far_side_reflection);
    RUN(noise_keeps_angle_close);
    RUN(strength_is_three_receiver_average);
    RUN(strength_wraps_neighbours_at_receiver_zero);
    RUN(strength_ripple_smaller_than_single_max);
    RUN(strength_scaling_and_clamp);
    RUN(strength_rises_as_ball_gets_closer);

    printf("\n%d checks, %d failures\n", g_checks, g_failures);
    return g_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
