#include <stdbool.h>
#include <string.h>
#include "game/applet.h"
#include "game/service_availability.h"

/* [INFERENCE] Role names: func_0019edcc is the SDK's pre-open check whose
 * failing result is reported through the fatal-error handler; func_00192b44
 * opens a named service port into *handle. The two status constants are the
 * "already open" and "open failed" results; their symbolic names are unknown. */

static __forceinline bool result_failed(unsigned result)
{
    return result >> 31;
}

unsigned func_0012e7f8(unsigned *handle, const char *name)
{
    unsigned result = func_0019edcc();
    if (result_failed(result))
        func_0019f568();
    if (*handle != 0)
        return 0xd8a103f9;
    result = func_00192b44(handle, name, strlen(name), 0);
    if ((int)(result & 0x80000000u) < 0)
        result = 0xd92103fb;
    return result;
}
