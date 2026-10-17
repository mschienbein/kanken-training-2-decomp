#include <stdbool.h>
#include "ctr/svc.h"
#include "game/applet.h"

/* Same declaration as src/sdk2/func_001380c8.c. */
extern CtrResult func_001380c8(unsigned int value0, unsigned int value1,
                              unsigned int value2, unsigned int value3, unsigned char *out);

static __forceinline bool result_failed(unsigned result)
{
    return result >> 31;
}

static CtrResult query_byte(unsigned char request, unsigned argument,
                             unsigned char mode, unsigned value, signed char *reply)
{
    return func_001380c8(request, argument, mode, value, (unsigned char *)reply);
}

/* [INFERENCE] Parameter roles are unrecovered; only each first/third argument's low byte is transmitted. */
int func_0012fdd0(unsigned char request, unsigned argument, unsigned char mode, unsigned value)
{
    signed char reply;
    unsigned result;
    unsigned address;
    func_0019ecf8();
    result = query_byte(request, argument, mode, value, &reply);
    address = __current_pc();
    if (result_failed(result))
        func_00192afc((CtrResult)result, (const void *)address);
    func_0019ec88();
    return reply;
}
