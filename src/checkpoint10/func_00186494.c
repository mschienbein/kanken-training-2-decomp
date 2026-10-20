#include <stddef.h>

struct ServiceStringCounts { int single_count; int group_count; };
extern struct ServiceStringCounts data_002430e4;
extern char bss_004b486c[];
extern size_t func_0019de40(char *destination, const char *source, size_t capacity);

void func_00186494(const char *text)
{
    /* ORIGINAL BUG: no capacity check guards the fixed global string table. */
    int offset = data_002430e4.single_count * 0x40;
    char *table = bss_004b486c;
    func_0019de40(table + offset, text, 0x40);
    ++data_002430e4.single_count;
}
