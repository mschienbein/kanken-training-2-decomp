#include "game/aux_effect.h"
#include "game/input_runtime.h"

void func_00146150(struct ServiceOutput *output, signed char index,
                   AuxCallback callback, void *context)
{
    output->callbacks[index] = callback;
    output->contexts[index] = context;
    func_0014b244(&bss_004b97ac, index, callback, context);
}
