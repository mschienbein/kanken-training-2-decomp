#include "game/service_state.h"
#include "game/service_runtime.h"

void func_00185f70(int enabled)
{
    if (enabled) {
        if (data_00242a28.completion) {
            func_001368b0();
            data_00242a28.completion = 0;
        }
    } else {
        if (func_0018f018()) {
            func_0018ef74();
            data_00242a28.completion = 1;
        }
    }
}
