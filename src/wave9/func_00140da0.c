#include "game/sound_format.h"

const struct SoundType1Details *func_00140da0(const struct SoundRecord *record)
{
    return (const struct SoundType1Details *)
        ((const unsigned char *)record + record->detail_offset);
}
