#include "game/sound_format.h"
#include "game/sound_format.h"
#include "game/archive.h"



bool func_00139358(const struct SoundArchiveReader *reader, unsigned item_id,
                  struct SoundType1Info *output)
{
    const struct SoundRecord *record = func_0018341c(reader->info, item_id);
    if (record == 0)
        return false;
    if (func_001833ec(record) != 1)
        return false;
    const struct SoundType1Details *details = func_00140da0(record);
    output->option_zero = func_00140c50(details);
    output->property = details->property;
    output->option_low = func_00140c70(details);
    output->option_high = func_00140c98(details);
    func_00140bf0(details, output->indices);
    return true;
}
