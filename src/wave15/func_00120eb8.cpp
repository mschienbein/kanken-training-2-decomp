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

extern "C" signed char func_00120eb8(void)
{
    unsigned handle = func_00192a7c();
    signed char value;
    Result result = func_0012e528(&handle, &value);
    unsigned address = (unsigned)__current_pc();
    check_result(result, address);
    /* ORIGINAL BUG: a returning error reporter leaves value uninitialized after failed IPC. */
    return value;
}
