#include "game/archive.h"
#include "game/sound_loader.h"



SoundLoaderState *func_001d4324(SoundLoaderState *loader)
{
    loader->vtable = &data_00224810;
    loader->archive = 0;
    return loader;
}
