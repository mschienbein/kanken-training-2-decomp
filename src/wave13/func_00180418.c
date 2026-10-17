#include "game/audio_voice.h"
unsigned int func_00180418(void)
{
    data_00243748 = data_00243748 * 1664525u + 1013904223u;
    return data_00243748 >> 16;
}
