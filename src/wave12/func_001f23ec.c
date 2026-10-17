#include "game/item_text.h"
#include "game/archive.h"


const struct W12cSoundDetail *func_001f23ec(const struct SoundRecord *record)
{
    return (const struct W12cSoundDetail *)
        ((const unsigned char *)record + record->detail_offset);
}
