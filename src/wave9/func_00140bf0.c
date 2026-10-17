#include "game/sound_format.h"

static const struct SoundIndexList *index_list(const struct SoundType1Details *details)
{
    return (const struct SoundIndexList *)
        ((const unsigned char *)details + details->indices_offset);
}

void func_00140bf0(const struct SoundType1Details *details, unsigned *output)
{
    const struct SoundIndexList *list = index_list(details);
    unsigned i;
    for (i = 0; i < 4; ++i) {
        if (list->count <= i)
            output[i] = ~0u;
        else
            output[i] = list->indices[i];
    }
}
