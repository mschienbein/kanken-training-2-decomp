#include "game/item_text.h"

/* [INFERENCE] The typed sound-detail owner is shared with both serialized readers. */
const struct W12cSoundOption *func_001f22e0(const struct W12cSoundDetail *info)
{
    if (info->option.offset != -1 && info->option.type == 0x2210)
        return (const struct W12cSoundOption *)
            ((const unsigned char *)info + info->option.offset);
    return 0;
}
