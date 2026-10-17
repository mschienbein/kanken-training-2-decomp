#include "game/audio_voice.h"
unsigned int func_00155d24(unsigned int bits)
{
    bits -= (bits >> 1) & 0x55555555u;
    bits = (bits & 0x33333333u) + ((bits >> 2) & 0x33333333u);
    bits = (bits + (bits >> 4)) & 0x0f0f0f0fu;
    bits += bits >> 8;
    bits += bits >> 16;
    return bits & 0x3fu;
}
