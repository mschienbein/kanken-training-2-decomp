#include "game/random.h"

extern int func_0017abf8(int channel, int sound);

void func_00173350(int kind)
{
    int sound;
    switch (kind) {
    case 0:
        sound = 0x01000010;
        break;
    case 1: {
        int choices[3] = {16777223, 16777229, 16777231};
        sound = choices[func_0018ffac(3)];
        break;
    }
    case 2: {
        int choices[2] = {16777223, 16777231};
        sound = choices[func_0018ffac(2)];
        break;
    }
    case 3: {
        int choices[4] = {16777235, 16777226, 16777230, 16777234};
        sound = choices[func_0018ffac(4)];
        break;
    }
    default:
        sound = 0x01000008;
        break;
    }
    if (sound != -1)
        func_0017abf8(1, sound);
}
