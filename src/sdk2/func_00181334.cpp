#include "game/archive.h"
extern "C" bool func_00181334(const SoundArchive *archive, SoundPlayerResult *out)
{
    bool result = func_0014a3c4(archive->reader, out);
    SoundArchiveAddon *addon = archive->addon;
    if (addon != 0)
        result |= addon->read_player(out);
    return result;
}
