#include "game/sound_format.h"

const struct SoundOptionalParameters *func_001f1904(const struct WaveSoundData *data)
{
    return (const struct SoundOptionalParameters *)
        ((const unsigned char *)data + data->options.offset);
}
