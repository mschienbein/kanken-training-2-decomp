#include "game/sound_format.h"

const struct WaveSoundData *func_00138fb0(const struct WaveSoundInfoBlock *info, unsigned index)
{
    const struct SoundReferenceTable *table = (const struct SoundReferenceTable *)
        ((const unsigned char *)info + info->wave_sound_data.offset);
    if (index >= table->count)
        return 0;
    if (table->entries[index].type != 0x4900)
        return 0;
    return (const struct WaveSoundData *)
        ((const unsigned char *)table + table->entries[index].offset);
}
