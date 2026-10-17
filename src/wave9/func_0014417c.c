#include "game/sound_format.h"

const struct SoundReferenceTable *func_0014417c(const struct SoundArchiveInfo *info)
{
    return (const struct SoundReferenceTable *)
        ((const unsigned char *)info + info->range_table_offset);
}
