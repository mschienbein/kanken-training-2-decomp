#include "game/sound_format.h"

const struct SoundType3Details *func_00140d6c(const struct SoundRecord *record)
{
    return (const struct SoundType3Details *)
        ((const unsigned char *)record + record->detail_offset);
}
