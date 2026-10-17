#include "game/audio_voice.h"

void func_0017e470(struct AudioVoiceGroup *group, unsigned int index,
                  const struct AdpcmCoefficients *source)
{
    func_001550e0(group->channels[index], source);
}
