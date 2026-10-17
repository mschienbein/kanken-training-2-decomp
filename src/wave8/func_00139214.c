#include "game/sound_format.h"

int func_00139214(const struct SoundArchiveReader *archive,
                   unsigned bank_id, unsigned *file_id)
{
    const struct SoundBankArchiveInfo *info = func_001409cc(archive->info, bank_id);
    if (info == 0)
        return 0;
    *file_id = info->file_id;
    return 1;
}
