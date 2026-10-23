/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr snd_Util.cpp and ut_Inlines.h,
 * commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd (THIRD_PARTY.md).
 * Changes: address-bound interfaces and shared scalar/table helpers.
 */
#include "game/audio_scale.h"

/* [INFERENCE] Shared helper factoring and TU membership are a code-generation
 * model, not recovered historical names. All helper uses are real conversions. */
static float clamp_scalar(float value, float minimum, float maximum);
static float lookup_ratio(const float *table, int index);

unsigned func_0014fe14(float value)
{
    value = clamp_scalar(value, 0.0f, 1.0f);
    unsigned short frequency;
    if (value < 0.135614381f)
        frequency = 80;
    else if (value >= 0.9f)
        frequency = 16000;
    else
        frequency = data_0021dfcc[(int)((value - 0.135614381f) / (0.1f / 3))];
    return frequency;
}

float func_0014feac(int pitch)
{
    int octave = 0;
    while (pitch < 0) {
        --octave;
        pitch += 12 * 256;
    }
    while (pitch >= 12 * 256) {
        ++octave;
        pitch -= 12 * 256;
    }
    const int note = pitch / 256;
    const int fraction = pitch - note * 256;
    float ratio = 1.0f;
    while (octave > 0) {
        ratio *= 2.0f;
        --octave;
    }
    while (octave < 0) {
        ratio /= 2.0f;
        ++octave;
    }
    if (note != 0)
        ratio *= lookup_ratio(data_0021dffc, note);
    if (fraction != 0)
        ratio *= lookup_ratio(data_0021e02c, fraction);
    return ratio;
}

float func_0014ffd8(float value)
{
    value = clamp_scalar(value, -90.4f, 6.0f);
    return lookup_ratio(data_0021e42c, 904 + (int)(10 * value));
}

static float clamp_scalar(float value, float minimum, float maximum)
{
    return value > maximum ? maximum : value < minimum ? minimum : value;
}

static float lookup_ratio(const float *table, int index)
{
    return table[index];
}
