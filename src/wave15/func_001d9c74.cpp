#include <stddef.h>
#include "game/audio_voice.h"
#include "game/sound_runtime.h"
#include "game/wave_player.h"



extern "C" W15iWaveSound *func_001d9c74(W15iWaveSound *sound)
{
    sound->base.vtable = data_00224aac;
    W15iWavePlayer *player = func_001d603c(&sound->player);
    sound = reinterpret_cast<W15iWaveSound *>(
        reinterpret_cast<char *>(player) - offsetof(W15iWaveSound, player));
    sound->base.vtable = data_0022459c;
    sound->base.active = 3;
    return sound;
}
