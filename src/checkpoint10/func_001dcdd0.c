#include "game/controller_transitions.h"
#include "game/screen.h"
#include "game/resource_tables.h"

struct DeferredResourceEntry16 {
    unsigned int key;
    unsigned int fields[3];
};

struct DeferredResourceList16 {
    unsigned char reserved00[0x0c];
    int state;
    struct DeferredResourceEntry16 *entries;
    int count;
};



void func_001dcdd0(struct DeferredResourceList16 *list)
{
    if (list->state) {
        if (list->state == -1) {
            list->state = 0;
        } else if (list->state == 1 && !func_0019efd0()) {
            int count;
            struct DeferredResourceEntry16 *entry;
            list->entries = (struct DeferredResourceEntry16 *)func_001971a0(15, 0);
            count = 0;
            entry = list->entries;
            for (;;) {
                if (!entry->key)
                    break;
                ++count;
                ++entry;
            }
            list->count = count;
            list->state = 2;
        }
    }
}
