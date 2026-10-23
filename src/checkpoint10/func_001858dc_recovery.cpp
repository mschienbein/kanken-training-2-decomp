#include "game/sound_format.h"


bool func_001858dc(const SoundArchive *archive, unsigned id, SoundBasicInfo *out)
{
    bool result = func_0013923c(archive->reader, id, out);
    SoundArchiveAddon *addon = archive->addon;
    if (addon != 0)
        result |= addon->read_sound(id, out);
    return result;
}
