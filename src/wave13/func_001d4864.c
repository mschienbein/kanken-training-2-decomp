#include "game/audio_voice.h"
static __forceinline unsigned adpcm_size(unsigned samples)
{
    unsigned size = (samples / 14) * 8;
    unsigned remainder = samples % 14;
    if (remainder != 0)
        size += (remainder + 1) / 2 + 1;
    return size;
}

unsigned func_001d4864(unsigned samples, int format)
{
    unsigned size = 0;
    switch (format) {
    case 0:
        size = samples;
        break;
    case 1:
        size = samples * 2;
        break;
    case 3:
        size = adpcm_size(samples);
        break;
    }
    return size;
}
