#include "game/service_runtime.h"
#include <new>


/* Recovered startup function constructing the initial error result. */
void func_0020e32c(void)
{
    new (&bss_0049f000.result) Result(0xe7e3ffffu);
}
