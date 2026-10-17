#include "game/sound_format.h"

const struct SoundEnvelope *func_001f4538(const struct SoundRegion *region)
{
    unsigned value;
    const struct SoundReference *reference;
    if (!func_0018279c(&region->option_flags, &value, 9))
        return &data_002437f8;
    reference = (const struct SoundReference *)((const unsigned char *)region + (int)value);
    return (const struct SoundEnvelope *)((const unsigned char *)reference + reference->offset);
}
