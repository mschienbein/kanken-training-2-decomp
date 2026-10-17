#include "game/audio_voice.h"

void func_001d49e8(struct AudioVoiceGroup *group, int index, const struct AudioSlot *slot)
{
    func_001c5994(group->channels[index], slot);
}
