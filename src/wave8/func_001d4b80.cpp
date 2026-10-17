#include "game/sound_runtime.h"

extern "C" void func_001d4b80(SoundParameterTarget *target, unsigned char value)
{
    target->flagf4 = value;
}
