#include "game/sound_format.h"

const struct SoundBankArchiveInfo *func_001409cc(const struct SoundArchiveInfo *file,
                                         unsigned bank_id)
{
    const struct SoundReferenceTable *table;
    if ((bank_id >> 24) != 3)
        return 0;
    bank_id &= 0x00ffffff;
    table = (const struct SoundReferenceTable *)
        ((const unsigned char *)file + file->third_table_offset);
    if (bank_id >= table->count)
        return 0;
    return (const struct SoundBankArchiveInfo *)
        ((const unsigned char *)table + table->entries[bank_id].offset);
}
