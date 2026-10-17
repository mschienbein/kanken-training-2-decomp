#include "game/audio_voice.h"

void func_001d911c(struct AudioVoice *voice, signed char mode)
{
    if (voice->group != 0) {
        int count = voice->group->count;
        int i;
        for (i = 0; i < count; ++i) {
            struct AudioSlot *slot = &voice->slots[i];
            if (slot->mode != mode) {
                slot->mode = mode;
                func_001d49e8(voice->group, i, slot);
            }
        }
    }
}
