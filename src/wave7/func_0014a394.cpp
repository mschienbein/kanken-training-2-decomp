#include "game/archive.h"


extern "C" bool func_0014a394(const SoundArchiveReader *reader, unsigned id, SoundFileResult *out)
{
    const SoundFileInfo *info = func_0014c25c(reader->info, id);
    if (info == 0)
        return false;
    out->count = info->count;
    out->size = func_0014c23c(info);
    return true;
}
