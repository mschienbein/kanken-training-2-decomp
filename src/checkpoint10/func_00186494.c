#include <stddef.h>

#include "game/service_state.h"
extern char bss_004b486c[];

void func_00186494(const char *text)
{
    /* ORIGINAL BUG: no capacity check guards the fixed global string table. */
    int offset = data_002430e4.single_count * 0x40;
    char *table = bss_004b486c;
    func_0019de40(table + offset, text, 0x40);
    ++data_002430e4.single_count;
}
