/* v9 no-frame probe: raw GetSystemTick service through the project svc.h.
 * Public ABI: https://www.3dbrew.org/wiki/SVC and https://www.3dbrew.org/wiki/Kernel_ABI */
#include "ctr/svc.h"

unsigned long long func_0011ef4c(void)
{
    return (unsigned long long)svcGetSystemTick();
}
