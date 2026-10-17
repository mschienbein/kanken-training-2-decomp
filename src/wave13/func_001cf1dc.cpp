#include "game/command_group.h"
extern "C" void func_001cf1dc(VoiceByteParameters140 *voice,
                              unsigned int index, unsigned char value)
{
    VoiceBytes4 *groups = (VoiceBytes4 *)(voice + 1);
    groups[index >> 2].values[index & 3] = value;
}
