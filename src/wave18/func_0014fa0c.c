#include "game/service_runtime.h"
#include "game/input_runtime.h"

void func_0014fa0c(struct ServiceOutput *output, int index, float value)
{
    if (output->active) {
        output->channels[index] = value;
        func_00152428(&bss_004b97ac, index, value);
    }
}
