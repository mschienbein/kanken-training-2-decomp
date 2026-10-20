#include "game/audio_voice.h"

extern "C" void func_001d911c(struct AudioVoice *voice, signed char mode)
{
    if (voice->group != 0) {
        int count = voice->group->count;
        int i;
        for (i = 0; i < count; ++i) {
            struct RecordState *slot = &voice->wave_buffers[i][0];
            if (slot->loop != mode) {
                slot->loop = mode;
                func_001d49e8(voice->group, i, slot);
            }
        }
    }
}
