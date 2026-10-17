#include "game/sound_runtime.h"

extern "C" SoundPlaybackState *func_001d3148(SequenceSound *sound)
{
    return &sound->playback;
}
