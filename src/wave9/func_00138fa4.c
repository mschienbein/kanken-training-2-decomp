#include "game/sound_format.h"

const struct WaveIdentifierTable *func_00138fa4(const struct WaveSoundInfoBlock *info)
{
    return (const struct WaveIdentifierTable *)
        ((const unsigned char *)info + info->wave_ids.offset);
}
