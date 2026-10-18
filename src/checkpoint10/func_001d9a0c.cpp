#include "game/sound_runtime.h"


void func_001d9a0c(WaveSoundControl *sound)
{
    func_0016a560(&sound->sound);
    sound->source = 0;
    sound->active = 1;
    sound->enabled = 0;
}
