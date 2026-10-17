#include "game/sound_format.h"

int func_0013931c(const struct SoundArchiveReader *reader, unsigned item_id,
                  struct SoundRangeInfo *output)
{
    const struct SoundRangeRecord *record = func_00182760(reader->info, item_id);
    if (record == 0)
        return 0;
    output->first_id = record->first_id;
    output->last_id = record->last_id;
    output->data = (const unsigned char *)record + record->name_offset;
    return 1;
}
