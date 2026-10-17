#include "game/sound_format.h"
#include "game/archive.h"




extern "C" bool func_001392b0(const struct SoundArchiveReader *reader, unsigned item_id,
                             struct SoundType3Info *output)
{
    const SoundRecord *record = func_0018341c(reader->info, item_id);
    if (record == 0)
        return false;
    if (func_001833ec(record) != 3)
        return false;
    const SoundType3Details *details = func_00140d6c(record);
    output->first = details->first;
    output->second = details->second;
    output->option_low = func_00140b9c(details);
    output->option_high_present = func_00140bc4(details) != 0;
    return true;
}
