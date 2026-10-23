/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr snd_Util.cpp, pinned commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd (THIRD_PARTY.md).
 * Changes: address-bound external lookup tables and recovered function ABI. */
extern const float data_0021dffc[12];
extern const float data_0021e02c[256];

extern "C" float func_0014feac(int pitch)
{
    int octave = 0;
    int note;
    int fraction;
    float ratio;
    while (pitch < 0) {
        --octave;
        pitch += 12 * 256;
    }
    while (pitch >= 12 * 256) {
        ++octave;
        pitch -= 12 * 256;
    }
    note = pitch / 256;
    fraction = pitch - note * 256;
    ratio = 1.0f;
    while (octave > 0) {
        ratio *= 2.0f;
        --octave;
    }
    while (octave < 0) {
        ratio /= 2.0f;
        ++octave;
    }
    if (note != 0)
        ratio *= data_0021dffc[note];
    if (fraction != 0)
        ratio *= data_0021e02c[fraction];
    return ratio;
}
