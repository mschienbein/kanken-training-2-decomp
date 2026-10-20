#include "game/input_runtime.h"


void func_00152428(struct InputSessionContext *settings, int index, float value)
{
    if (settings->active) {
        settings->channels[index] = value;
        func_00180d9c(&bss_004b5284, index, value);
    }
}
