#include "game/sound_format.h"

int func_0013923c(const struct SoundArchiveReader *reader, unsigned item_id,
                  struct SoundBasicInfo *output)
{
    const struct SoundRecord *record = func_0018341c(reader->info, item_id);
    if (record == 0)
        return 0;
    output->item_id = record->item_id;
    output->field04 = record->field04;
    output->property = record->property;
    output->option_one_low = func_00140ccc(record);
    output->option_one_high = func_00140cf0(record);
    output->option_two_low = func_00140d78(record);
    output->option_two_high = func_00140d40(record);
    output->flag = func_00140d1c(record);
    return 1;
}
