#include "game/input_runtime.h"


void func_00146188(struct InputController *controller)
{
    int index;
    for (index = 0; index < 2; ++index) {
        signed char channel = (signed char)index;
        controller->primary[channel].flags = 0xffff;
        func_00180fe0(&bss_004b5284, channel, &controller->primary[channel]);
        controller->channels[channel].flags = 0xffff;
        func_00180e74(&bss_004b5284, channel, &controller->channels[channel]);
    }
}
