#include "game/controller_transitions.h"
#include "game/screen.h"
#include "game/resource_tables.h"

#include "game/selection_components.h"



void func_001dcf4c(struct DeferredResourceList20 *list)
{
    if (list->state) {
        if (list->state == -1) {
            list->state = 0;
        } else if (list->state == 1 && !func_0019efd0()) {
            struct DetailEntry *entry;
            int count;
            list->entries = (struct DetailEntry *)func_001971a0(15, 1);
            count = 0;
            for (entry = list->entries; entry->id; ++entry)
                ++count;
            list->count = count;
            list->state = 2;
        }
    }
}
