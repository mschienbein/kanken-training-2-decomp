#include "game/runtime.h"
typedef struct ResourceEntry {
    void *resource;
    unsigned char reserved[0xd0];
} ResourceEntry;

typedef struct ResourceGroup {
    unsigned int reserved;
    ResourceEntry entries[8];
} ResourceGroup;

typedef struct ResourceState {
    unsigned char reserved[0x18];
    ResourceGroup groups[2];
} ResourceState;
extern ResourceState bss_0049cbd0;

void *func_00196cb8(int screen, int index)
{
    int group = -1;
    ResourceEntry *entry;
    unsigned char *word;

    if (screen == 3)
        group = 0;
    else if (screen == 4)
        group = 1;
    else
        func_0019f568();

    /* ORIGINAL BUG: an invalid screen still indexes before the group table. */
    word = (unsigned char *)&bss_0049cbd0;
    word += (short)group * sizeof(ResourceGroup);
    word += index * sizeof(ResourceEntry);
    entry = (ResourceEntry *)(word + 0x1c);
    if (entry)
        return entry->resource;
    func_0019f568();
    return 0;
}
