#include "game/result.h"
#include "game/service_clients.h"



static __forceinline void check_result(Result result, unsigned address)
{
    switch (result.failed()) {
    case 0:
        break;
    default:
        func_0019d6c4(result.value, address);
    }
}

extern "C" void func_0019df5c(void)
{
    Result result = func_00121444();
    if ((int)(result.value & 0x80000000u) < 0) {
        unsigned address = (unsigned)__current_pc();
        check_result(result, address);
    }
}
