#include "game/service_result.h"
#include "game/service_availability.h"

/* [INFERENCE] These local request values package the two forwarded triples.
 * They are not external types: both endpoints retain their evidenced flat ABI. */
struct RequestFields0 { unsigned a10; const void * a11; unsigned a12; };
struct RequestFields1 { const void * a5; unsigned a6; void * a7; };

Result func_0012f050(
    unsigned argument1, unsigned argument2, const void *argument3,
    unsigned argument4, const void *argument5, unsigned argument6,
    void *argument7, unsigned argument8, void *argument9,
    unsigned argument10, const void *argument11, unsigned argument12,
    void *argument13, unsigned argument14, void *argument15,
    unsigned argument16, unsigned *argument17, void *argument18,
    unsigned argument19)
{
    RequestFields0 fields0 = { argument10, argument11, argument12 };
    RequestFields1 fields1 = { argument5, argument6, argument7 };
    unsigned count = argument14;
    void *buffer = argument18;
    if (!func_00186538())
        return Result(0xc8a0d018u);
    return func_00136f38(
        argument1, argument2, argument3,
        argument4, fields1.a5, fields1.a6,
        fields1.a7, argument8, argument9,
        fields0.a10, fields0.a11, fields0.a12,
        argument13, count, argument15,
        argument16, argument17, buffer,
        buffer, argument19);
}
