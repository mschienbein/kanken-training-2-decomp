#include "game/aux_effect.h"
#include "game/input_runtime.h"

void func_0014c990(struct ServiceOutput *output, signed char index)
{
    output->callbacks[index] = 0;
    output->contexts[index] = 0;
    func_0014b244(&bss_004b97ac, index, 0, 0);
}
