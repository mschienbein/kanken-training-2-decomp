#include "game/sound_archive.h"
#include "game/sound_format.h"
#include "game/archive.h"






int SoundArchiveAdapter::readRange(unsigned item_id, SoundRangeInfo *output) const
{
    int result = func_0013931c(reader, item_id, output);
    if (supplement != 0)
        result |= supplement->readRangeIfEnabled(item_id, output);
    return result;
}
