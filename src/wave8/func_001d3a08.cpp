#include "game/sound_runtime.h"

extern "C" SoundPlaybackState *func_001d3a08(WaveSound *sound)
{
    return &sound->playback;
}
