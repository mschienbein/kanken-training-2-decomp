#include "game/archive.h"
extern "C" bool func_0014a298(const SoundArchive *archive, unsigned id, SoundFileResult *out)
{
    bool result = func_0014a394(archive->reader, id, out);
    SoundArchiveAddon *addon = archive->addon;
    if (addon != 0)
        result |= addon->read_file(id, out);
    return result;
}
