#include "game/sound_format.h"

const struct SoundReferenceTable *func_001f4500(const struct SoundBankInfo *bank)
{
    return (const struct SoundReferenceTable *)((const unsigned char *)bank + bank->instrument_table.offset);
}
