#include "game/applet.h"
#include <stdbool.h>


static __forceinline bool result_failed(unsigned result)
{
    return result >> 31;
}

int func_0012fda0(unsigned int flags)
{
    signed char registered;
    unsigned result;
    const void *address;
    func_0019ecf8();
    /* The endpoint writes one representation byte; the result is signed. */
    result = func_0018f068(flags, (unsigned char *)&registered);
    address = (const void *)__current_pc();
    if (result_failed(result))
        func_00192afc(result, address);
    func_0019ec88();
    return registered;
}
