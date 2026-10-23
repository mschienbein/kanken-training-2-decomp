#include "game/audio_voice.h"
/* [INFERENCE] The eight-byte constructor and paired writes in
 * func_001d5570 describe a 64-bit counter. */


void *func_00156dac(void *storage)
{
    AudioCounter *counter = static_cast<AudioCounter *>(storage);
    counter->value = 0;
    return counter;
}
