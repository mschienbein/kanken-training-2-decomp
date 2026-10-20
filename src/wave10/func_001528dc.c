#include "game/prefetch_stream.h"

void func_001528dc(struct PrefetchFileReader *reader, const struct PrefetchFileHeader *file)
{
    const struct PrefetchFileBlock *info;
    const struct PrefetchFileBlock *data;
    if (file->signature != 0x50545343 || (file->version - 0x10000u) > 0x10200u)
        return;
    reader->file = file;
    info = func_00153ebc(file);
    /* ORIGINAL BUG: a missing block is dereferenced before its signature is checked. */
    if (info->signature != 0x4f464e49)
        return;
    data = func_00153f28(reader->file);
    if (data->signature != 0x54414450)
        return;
    reader->info = (const struct PrefetchInfoBlock *)(info + 1);
    reader->data = (const struct PrefetchDataBlock *)(data + 1);
}
