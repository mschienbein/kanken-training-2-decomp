/* SPDX-License-Identifier: GPL-3.0-only
 * Helper layering adapted from NintendoWare4Ctr contributors,
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd, sources/libraries/snd/snd_Voice.cpp.
 * Modified: canonical project structs and address-bound calls.
 * See src/nw4c/LICENSE.txt and THIRD_PARTY.md.
 */
/* [INFERENCE] Natural refresh-helper layering; historical TU membership and
 * callback class identity remain unrecovered. The unrelated pending-queue
 * mismatch is preserved in the immutable author TU, not emitted here. */
#include "game/audio_voice.h"
#include "game/audio_tasks.h"
#include "game/aux_effect.h"

/* [INFERENCE] Callback identity; the original dispatch passes the type, scalar
 * value and writable coefficient destination through the third vtable slot. */
class AudioAuxCalculatorTU856 {
public:
    virtual ~AudioAuxCalculatorTU856();
    virtual void calculate(int type, float value,
                           AudioBiquadCoefficients *output) const = 0;
};

/* [INFERENCE] Shared dirty-property accessors with constant masks. */
static bool property_is_dirty(const AudioVoiceGroup *group, unsigned mask)
{
    return (group->dirty & mask) != 0;
}
static void clear_property(AudioVoiceGroup *group, unsigned mask)
{
    group->dirty &= ~mask;
}


/* [INFERENCE] Native CalcSrc has an initialization selector; this target
 * version propagates the same pitch for both initialization modes. */
static void refresh_pitch(AudioVoiceGroup *group, bool initial)
{
    (void)initial;

    float pitch = group->value28;
    if (pitch > 255.0f)
        pitch = 255.0f;
    for (int channel = 0; channel < group->count; ++channel)
        if (group->channels[channel])
            func_0014fc78(group->channels[channel], pitch);
}

static void refresh_volume(AudioVoiceGroup *group)
{
    float volume = group->value24;
    /* The manager's first 20 bytes are the canonical AudioLinearRamp view. */
    volume *= func_00151a1c(&bss_004b32c0.master_volume);
    for (int channel = 0; channel < group->count; ++channel)
        if (group->channels[channel])
            func_0015274c(group->channels[channel], volume);
}

static void refresh_lpf(AudioVoiceGroup *group)
{
    unsigned frequency = func_0014fe14(group->value38);
    for (int channel = 0; channel < group->count; ++channel) {
        AudioChannel *voice = group->channels[channel];
        if (voice) {
            if (frequency >= 16000) {
                func_001525f0(voice, false);
            } else {
                func_001525f0(voice, true);
                func_0014fbec(voice, frequency);
            }
        }
    }
}

static void refresh_biquad(AudioVoiceGroup *group)
{
    for (int channel = 0; channel < group->count; ++channel) {
        AudioChannel *voice = group->channels[channel];
        if (voice) {
            const AudioAuxCalculatorTU856 *calculator =
                static_cast<const AudioAuxCalculatorTU856 *>(bss_004b32c0.tables[group->reserved22]);
            if (!calculator || !(group->value3c > 0.0f)) {
                func_0014fbcc(voice, false);
            } else {
                AudioBiquadCoefficients parameters;
                func_0014fbcc(voice, true);
                calculator->calculate(group->reserved22, group->value3c, &parameters);
                func_0015263c(voice, &parameters);
            }
        }
    }
}

void func_0014eec8(AudioVoiceGroup *group)
{
    if (!group->flag15)
        return;
    if (property_is_dirty(group, 4)) {
        refresh_pitch(group, false);
        clear_property(group, 4);
    }
    if (property_is_dirty(group, 0x40)) {
        refresh_volume(group);
        clear_property(group, 0x40);
    }
    if (property_is_dirty(group, 8)) {
        func_00150124(group);
        clear_property(group, 8);
    }
    if (property_is_dirty(group, 0x10)) {
        refresh_lpf(group);
        clear_property(group, 0x10);
    }
    if (property_is_dirty(group, 0x20)) {
        refresh_biquad(group);
        clear_property(group, 0x20);
    }
}

