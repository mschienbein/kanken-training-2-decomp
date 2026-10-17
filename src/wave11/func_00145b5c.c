#include "game/screen.h"
#include "game/runtime.h"






struct BoundDisplayObject *func_00145b5c(int screen, int index)
{
    int group = -1;
    struct ResourceRegistrationSlot *entry;
    unsigned char *word;

    if (screen == 3)
        group = 0;
    else if (screen == 4)
        group = 1;
    else
        func_0019f568();

    /* ORIGINAL BUG: an invalid screen still indexes before the group table. */
    word = (unsigned char *)&bss_0049cbd0;
    word += (short)group * sizeof(struct ResourceRegistrationGroup);
    word += index * sizeof(struct ResourceRegistrationSlot);
    entry = (struct ResourceRegistrationSlot *)(word + 0x1c);
    /* ORIGINAL BUG: this tests the embedded address rather than slot validity. */
    if (entry)
        return &entry->display;
    func_0019f568();
    return 0;
}
