#include "game/audio_voice.h"
void func_0016a158(struct AudioEnvelope *envelope, int value)
{
    int square = (value + 1) * (value + 1);
    envelope->decay = square / 4;
}
