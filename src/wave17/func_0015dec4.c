#include "game/screen.h"

/* func_00177b10 registers the no-input, void func_001b7dac callback. */

void func_0015dec4(void (*callback)(void))
{
    data_0023ee9c.pending = 0;
    data_0023ee9c.callback = callback;
}
