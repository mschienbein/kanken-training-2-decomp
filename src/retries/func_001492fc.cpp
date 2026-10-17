#include "game/sound_voice.h"
#include "game/pool_list.h"



SoundStreamInstance::SoundStreamInstance(SoundItemPool *pool) : owner(pool)
{
    active = false;
    field3290 = 0;
    field3294 = 0;
}
