/* SPDX-License-Identifier: GPL-3.0-only
 * Arithmetic follows NintendoWare4Ctr snd_Util.cpp / ut_Inlines.h,
 * commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd (THIRD_PARTY.md).
 */
#include "game/audio_voice.h"
static float clamp_scalar(float value, float minimum, float maximum);

unsigned func_0014fe14(float value)
{
    value = clamp_scalar(value, 0.0f, 1.0f);
    unsigned short frequency;
    if (value < 0.135614381f) frequency = 80;
    else if (value >= 0.9f) frequency = 16000;
    else frequency = data_0021dfcc[(int)((value - 0.135614381f) / (0.1f / 3))];
    return frequency;
}

static float clamp_scalar(float value, float minimum, float maximum)
{
    return value > maximum ? maximum : value < minimum ? minimum : value;
}
