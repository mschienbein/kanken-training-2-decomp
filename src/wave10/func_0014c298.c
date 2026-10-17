#include "game/archive.h"

const unsigned *func_0014c298(const struct SoundArchiveInfo *header)
{
    return (const unsigned *)((const char *)header + header->count_table_offset);
}
