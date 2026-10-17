#include "game/scene_lifecycle.h"
#include "game/audio_voice.h"




static __forceinline void stop_voice(AudioVoice *voice)
{
    if (voice->group == 0)
        return;
    func_001835b8(voice->group);
    func_00183550(voice->group);
    voice->group = 0;
    voice->flag138 = 0;
    voice->flag139 = 0;
    if (voice->callback != 0)
        voice->callback(voice, 0, voice->callback_data);
    if (voice->flag13a != 0) {
        voice->flag13a = 0;
        func_00182994(func_00185cc0(), voice);
    }
}

extern "C" void func_001d9580(struct AudioVoiceOwner001d9580 *owner,
                             const unsigned char *first, const unsigned char *last)
{
    int channel;
    int buffer;
    int overlap;
    int count;

    if (owner->voice->group == 0)
        return;
    count = owner->voice->group->count;
    overlap = 0;
    for (channel = 0; count > channel; ++channel) {
        for (buffer = 0; buffer < 2; ++buffer) {
            struct AudioSlotBufferDescriptor001d9580 *descriptor =
                &((struct AudioSlotBufferDescriptor001d9580 *)&owner->voice->slots[channel])[buffer];
            if (descriptor->state != 0 && descriptor->state != 3) {
                const unsigned char *begin = descriptor->address;
                unsigned char format = func_001f2f2c(owner->voice->group);
                const unsigned char *end = begin + func_001d4864(descriptor->sample_count, format);
                if (first <= end && descriptor->address <= last) {
                    overlap = 1;
                    break;
                }
            }
        }
    }
    if (!overlap)
        return;

    if (owner->voice->callback != 0) {
        owner->voice->callback(owner->voice, 3, owner->voice->callback_data);
        owner->voice->callback = 0;
    }
    stop_voice(owner->voice);
}
