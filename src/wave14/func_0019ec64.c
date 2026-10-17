#include "game/applet.h"
#include "game/service_state.h"
#include <stdbool.h>


static __forceinline bool result_failed(unsigned result)
{
    return result >> 31;
}

void func_0019ec64(void)
{
    unsigned result;
    const void *address;
    func_0019ecf8();
    result = func_0012fee8(func_00191dd8());
    address = (const void *)__current_pc();
    if (result_failed(result))
        func_00192afc(result, address);
    func_0019ec88();
}
