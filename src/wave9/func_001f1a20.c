#include "game/sound_format.h"

const struct SoundEnvelope *func_001f1a20(const struct SoundOptionalParameters *note)
{
    unsigned offset;
    const struct SoundReference *reference;
    if (!func_0018279c(&note->flags, &offset, 9))
        return &data_0024380a;
    reference = (const struct SoundReference *)
        ((const unsigned char *)note + (int)offset);
    return (const struct SoundEnvelope *)
        ((const unsigned char *)reference + reference->offset);
}
