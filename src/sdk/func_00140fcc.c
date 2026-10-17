#include "ctr/cp15.h"

void **func_00140fcc(void)
{
    return (void **)(ctrGetThreadLocalStorage() + 0x5c);
}
