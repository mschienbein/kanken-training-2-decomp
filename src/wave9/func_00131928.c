#include "game/sound_format.h"

int func_00131928(const struct SoundArchiveReader *reader, unsigned item_id)
{
    const struct SoundRecord *record = func_0018341c(reader->info, item_id);
    if (record != 0)
        return func_001833ec(record);
    return 0;
}
