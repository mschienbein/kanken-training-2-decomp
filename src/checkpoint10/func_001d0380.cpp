#include "game/sound_runtime.h"


extern "C" unsigned char func_001d0380(SoundArchivePlayer *player,
    SoundHandle *handle, unsigned sound_id, int mode, const SoundStartInfo *info)
{
    return (unsigned char)func_001d0614(player, handle, sound_id, 0, 0, mode, info);
}
