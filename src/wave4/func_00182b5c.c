#include "game/functions.h"
#include "game/sdk_init.h"


static int ContainsRange(unsigned base, unsigned size, unsigned start, unsigned end)
{
    unsigned limit = base + size;
    return base <= start && start <= limit &&
           base <= end && end <= limit && start < end;
}

unsigned func_00182b5c(unsigned address, unsigned size)
{
    unsigned end = address + size;
    unsigned base = func_0019d058();
    unsigned span_size = func_0019d048();
    if (ContainsRange(base, span_size, address, end))
        return data_00243798.second + address;
    return 0;
}
