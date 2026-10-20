#include "game/archive.h"


bool func_001f16f8(const SoundArchive *archive, unsigned id, SoundArchiveWaveInfo *out)
{
    bool result = func_001f2b04(archive->reader, id, out);
    SoundArchiveAddon *addon = archive->addon;
    if (addon != 0)
        result |= addon->read_wave_archive(id, out);
    return result;
}
