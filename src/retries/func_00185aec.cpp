#include "game/sound_archive.h"
#include "game/archive.h"





int SoundArchiveAccessors::readBankFileId(unsigned bank_id, unsigned *file_id) const
{
    int result = func_00139214(archive, bank_id, file_id);
    if (hook != 0)
        result |= hook->readBankFileIdIfEnabled(bank_id, file_id);
    return result;
}
