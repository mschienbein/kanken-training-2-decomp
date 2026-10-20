#include "game/sound_task.h"

/* The original constructor and this getter occupy the same concrete type table. */
extern "C" SoundPlayerBase *func_001d3a08(EmbeddedSoundInstance *sound)
{
    return &sound->player.base;
}
