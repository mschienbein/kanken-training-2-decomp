#include "game/functions.h"
#include <stdint.h>
#include "game/shared_memory.h"





unsigned func_001378d8(const void *pointer)
{
    unsigned address = (uintptr_t)pointer;
    unsigned translated = func_00182b5c(address, 1);
    unsigned base;
    unsigned end;
    if (translated)
        return translated;
    if (address - 0x1f000000u < 0x600000u || address == 0x1f600000u)
        return address - 0x07000000u;
    if (func_0013e6b8()) {
        translated = func_0013e6d0(address, 1);
        if (translated)
            return translated;
    }
    base = (uintptr_t)func_0019dc04();
    end = base + func_0013e71c();
    if (base && base <= address && address < end) {
        if (base < 0x30000000u)
            return address + 0x0c000000u;
        return address - 0x10000000u;
    }
    return 0;
}
