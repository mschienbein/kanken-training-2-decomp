#include "game/archive.h"
#include "game/sound_loader.h"



void func_001d4124(CachedSoundLoaderState *owner, const SoundArchive *archive)
{
    if (owner->active)
        return;
    owner->active = true;
    owner->released = false;
    for (int i = 0; i < 9; ++i) {
        owner->cache[i].file_id = ~0u;
        owner->cache[i].file = 0;
    }
    func_001d4174(&owner->loader, archive);
}
