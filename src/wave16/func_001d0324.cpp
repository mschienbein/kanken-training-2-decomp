#include <stddef.h>
#include "game/archive.h"
#include "game/runtime.h"
#include "game/sound_loader.h"



void func_001d0324(DirectSoundLoaderState *owner)
{
    SoundLoaderState *base = func_001d4324(&owner->loader);
    func_001852f4(reinterpret_cast<char *>(base) - offsetof(DirectSoundLoaderState, loader));
}
