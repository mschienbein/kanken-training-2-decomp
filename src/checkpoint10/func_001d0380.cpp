#include "game/sound_runtime.h"

struct SoundArchivePlayer;
struct SoundStartInfo;
struct SoundStartActor;

/* INTEGRATOR: move to include/game/sound_runtime.h */
extern "C" {
unsigned func_001d0614(SoundArchivePlayer *player, SoundHandle *handle,
    unsigned sound_id, SoundInstance *attached, SoundStartActor *actor,
    int mode, const SoundStartInfo *info);
}

extern "C" unsigned char func_001d0380(SoundArchivePlayer *player,
    SoundHandle *handle, unsigned sound_id, int mode, const SoundStartInfo *info)
{
    return (unsigned char)func_001d0614(player, handle, sound_id, 0, 0, mode, info);
}
