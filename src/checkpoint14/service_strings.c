#include "game/service_state.h"

static void copy_service_fields(struct ServiceStringCounts *counts, char (*table)[0x40], const char *fourth, const char *fifth)
{
    func_0019de40(table[counts->group_count * 5 + 3], fourth, 0x40);
    func_0019de40(table[counts->group_count * 5 + 4], fifth, 0x40);
}

void func_00136450(const char *first, const char *second, const char *third,
                   const char *fourth, const char *fifth)
{
    struct ServiceStringCounts *counts = &data_002430e4;
    char (*table)[0x40] = bss_004b4a6c;
    /* ORIGINAL BUG: no capacity check guards the fixed global string table. */
    func_0019de40(table[counts->group_count * 5], first, 0x40);
    func_0019de40(table[counts->group_count * 5 + 1], second, 0x40);
    func_0019de40(table[counts->group_count * 5 + 2], third, 0x40);
    copy_service_fields(counts, table, fourth, fifth);
    ++counts->group_count;
}
