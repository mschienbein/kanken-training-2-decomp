#include <stdint.h>
#include "game/shared_memory.h"

struct ResourceTableEntry;
struct FontResource;
/* INTEGRATOR: differs from include/game/resource_tables.h: the four words at
 * 0x10..0x1c inside reserved0c are independently reset by this function. */
struct ResourceTable {
    struct ResourceTableEntry *entries;
    int count;
    int activeCount;
    unsigned char reserved0c[4];
    unsigned field10, field14, field18, field1c;
    unsigned char reserved20[0x14];
    struct FontResource *font;
    unsigned char reserved38[0x14];
    void *defaultSheets;
    unsigned char firstEnabled;
    unsigned char reserved51[3];
    void *firstSheets;
    uintptr_t firstResource;
    int firstIndex;
    unsigned char secondEnabled;
    unsigned char reserved61[3];
    void *secondSheets;
    uintptr_t secondResource;
    int secondIndex;
};
extern struct ResourceTable bss_002d3d60;

void func_00103978(void)
{
    func_0010bf88();
    bss_002d3d60.entries = 0;
    bss_002d3d60.field10 = 0;
    bss_002d3d60.field14 = 0;
    bss_002d3d60.field18 = 0;
    bss_002d3d60.font = 0;
    bss_002d3d60.defaultSheets = 0;
    bss_002d3d60.field1c = 0;
}
