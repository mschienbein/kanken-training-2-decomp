#include "game/font_resource.h"
#include "game/item_text.h"
#include "game/sound_format.h"
#include "game/archive.h"




int func_001f2e3c(const struct SoundArchiveReader *reader, unsigned sound_id,
                  struct W12cSoundOptionResult *out)
{
    const struct SoundRecord *record = func_0018341c(reader->info, sound_id);
    if (record == 0)
        return 0;
    if (func_001833ec(record) != 2)
        return 0;
    if (reader->version >= 0x02030100u) {
        const struct W12cSoundOption *option = func_001f22e0(func_001f23ec(record));
        unsigned char enabled;
        if (option == 0)
            return 0;
        enabled = (unsigned char)((option->flags & 0xffff) >> 8);
        if (enabled != 1)
            enabled = 0;
        out->enabled = enabled;
        out->first = option->first;
        out->second = option->second;
    } else {
        const struct BankInstrumentInfo *info = func_001f23f8(record);
        out->enabled = func_001f23b8(info);
        out->first = func_001f2354(info);
        out->second = func_001f2334(info);
    }
    return 1;
}
