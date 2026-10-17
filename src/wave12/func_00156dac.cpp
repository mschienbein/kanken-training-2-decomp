#include "game/audio_voice.h"
/* [INFERENCE] The eight-byte constructor and paired writes in
 * func_001d5570 describe a 64-bit counter. */


AudioCounter *func_00156dac(AudioCounter *counter)
{
    counter->value = 0;
    return counter;
}
