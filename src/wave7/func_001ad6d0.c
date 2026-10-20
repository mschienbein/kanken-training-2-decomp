#include "game/messages.h"


void func_001ad6d0(struct MessageLayout *layout, int first, int second)
{
    struct MessageEntry *entry = layout->entries[2];
    if (entry != 0 && entry->object != 0)
        func_00180358(entry->object, first, second);
}
