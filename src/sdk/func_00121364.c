#include "game/input_device.h"
#include "ctr/cp15.h"

void func_00121364(void)
{
    unsigned *slots = (unsigned *)ctrGetThreadLocalStorage();
    slots[0] = 0;
    slots[1] = 0;
    slots[2] = 0;
    slots[3] = 0;
    slots[4] = 0;
    slots[5] = 0;
    slots[6] = 0;
    slots[7] = 0;
    slots[8] = 0;
    slots[9] = 0;
    slots[10] = 0;
    slots[11] = 0;
    slots[12] = 0;
    slots[13] = 0;
    slots[14] = 0;
    slots[15] = 0;
}
