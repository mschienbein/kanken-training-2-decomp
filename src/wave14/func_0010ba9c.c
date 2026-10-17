#include "game/runtime.h"
#include "game/service_state.h"
#include "game/pool_list.h"



static __forceinline void check_result(unsigned result, unsigned address)
{
    switch (result >> 31) {
    case 0:
        break;
    default:
        func_0019d6c4(result, address);
        break;
    }
}

void func_0010ba9c(void *context)
{
    unsigned result;
    bss_004b1c74.context = context;
    result = func_0019ed2c(&bss_004b1c74, 0x207);
    check_result(result, __current_pc());
    result = func_00113ddc(0x207);
    check_result(result, __current_pc());
}
