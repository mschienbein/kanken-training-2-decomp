/* SPDX-License-Identifier: GPL-3.0-only
 * Helper layering adapted from NintendoWare4Ctr contributors,
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd, sources/libraries/snd/snd_Voice.cpp.
 * Modified: canonical project structs and address-bound calls.
 * See src/nw4c/LICENSE.txt and THIRD_PARTY.md.
 */
#include "game/audio_voice.h"
#include "game/audio_tasks.h"
#include "game/aux_effect.h"

enum AudioChannelAction {
    AUDIO_ACTION_NONE,
    AUDIO_ACTION_START,
    AUDIO_ACTION_STOP,
    AUDIO_ACTION_PAUSE
};

/* [INFERENCE] Native CalcSrc has an initialization selector; this target
 * version propagates the same pitch for both initialization modes. */
static void refresh_pitch(AudioVoiceGroup *group, bool initial)
{
    (void)initial;
    float volume = group->value28;
    if (volume > 255.0f)
        volume = 255.0f;
    for (int i = 0; i < group->count; ++i)
        if (group->channels[i])
            func_0014fc78(group->channels[i], volume);
}

static void refresh_volume(AudioVoiceGroup *group)
{
    float gain = group->value24;
    gain *= func_00151a1c(&bss_004b32c0.master_volume);
    for (int i = 0; i < group->count; ++i)
        if (group->channels[i])
            func_0015274c(group->channels[i], gain);
}

static void run_all_channels(AudioVoiceGroup *group)
{
    for (int i = 0; i < group->count; ++i)
        if (group->channels[i])
            func_001526b4(group->channels[i], 0);
}

static void stop_all_channels(AudioVoiceGroup *group)
{
    for (int i = 0; i < group->count; ++i)
        if (group->channels[i])
            func_001526b4(group->channels[i], 1);
}

static void pause_all_channels(AudioVoiceGroup *group)
{
    for (int i = 0; i < group->count; ++i)
        if (group->channels[i])
            func_001526b4(group->channels[i], 2);
}

void func_0014f114(struct AudioVoiceGroup *group)
{
    enum AudioChannelAction action;
    if (!group->active)
        return;
    action = AUDIO_ACTION_NONE;

    if ((group->dirty & 1) && group->flag15 && !group->flag16) {
        refresh_pitch(group, true);
        func_00150124(group);
        refresh_volume(group);
        group->flag16 = 1;
        action = AUDIO_ACTION_START;
        group->dirty &= ~0x0d;
    }

    if (group->flag16) {
        if ((group->dirty & 2) && group->flag15) {
            if (group->flag17) {
                group->flag18 = 1;
                action = AUDIO_ACTION_PAUSE;
            } else {
                group->flag18 = 0;
                action = AUDIO_ACTION_START;
            }
            group->dirty &= ~2;
        }
    }

    switch (action) {
    case AUDIO_ACTION_START:
        run_all_channels(group);
        break;
    case AUDIO_ACTION_STOP:
        stop_all_channels(group);
        break;
    case AUDIO_ACTION_PAUSE:
        pause_all_channels(group);
        break;
    default:
        break;
    }
}
