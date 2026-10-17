#include "game/sound_format.h"

const struct BankWaveArchiveTable *func_00140fc0(const struct SoundReference *reference)
{
    return (const struct BankWaveArchiveTable *)
        ((const unsigned char *)reference + reference->offset);
}
