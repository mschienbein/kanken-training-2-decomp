#include "game/audio_voice.h"
#include "game/pool_list.h"


extern "C" unsigned char func_001f2f2c(const nw::snd::internal::driver::Voice *group)
{
    return group->channels[0] != 0 ? group->format : 1;
}
