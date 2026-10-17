#include "game/sound_runtime.h"

extern "C" SoundPlaybackState *func_001d9b94(StreamSound *sound)
{
    return &sound->playback;
}
