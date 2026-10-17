#include "game/sound_runtime.h"

extern "C" void func_001d4d54(SoundParameterTarget *target, unsigned char value)
{
    target->flagf0 = value;
}
