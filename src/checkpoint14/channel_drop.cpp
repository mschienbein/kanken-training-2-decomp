#include "game/audio_voice.h"
#include "game/scene_lifecycle.h"

/* [INFERENCE] Canonical callback and state fields are retained; descriptive
 * field names do not recover the historical callback/class identity. */
static void drop_hardware_channel(unsigned char allocating, AudioVoiceGroup *voice, AudioChannel *dropped)
{
    if (allocating) {
        voice->allocation_failed = true;
        const int &channel_count = voice->count;
        int i = 0;
        if (channel_count > 0) {
            do {
                if (voice->channels[i] == dropped) {
                    voice->channels[i] = 0;
                    return;
                }
            } while (++i < channel_count);
        }
    } else {
        for (int i = 0; i < voice->count; ++i) {
            AudioChannel *channel = voice->channels[i];
            if (channel) {
                if (channel != dropped) {
                    func_001526b4(channel, 1);
                    func_001829d0(channel);
                }
                voice->channels[i] = 0;
            }
        }
    }
    voice->flag17 = false;
    voice->flag15 = false;
    voice->count = 0;
    func_0017e764(func_00185c58(), voice);
    voice->active = false;
    if (voice->callback)
        voice->callback(voice, 3, voice->context);
}

void func_00154674(AudioChannel *element, AudioVoiceGroup *group)
{
    AudioVoiceGroup *voice = group;
    drop_hardware_channel(voice->allocating, voice,
                          element);
}

