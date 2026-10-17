#include "game/archive.h"

const unsigned *func_0014c2b0(const struct SoundArchiveInfo *header)
{
    return (const unsigned *)((const char *)header + header->file_table_offset);
}
