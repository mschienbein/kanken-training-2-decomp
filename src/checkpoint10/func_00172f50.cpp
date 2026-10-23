#include "game/text_resources.h"

extern "C" void func_00172f50(OwnedElementListW22E *list)
{
    if (list->count > 0 && list->entries != 0) {
        for (int i = 0; i < list->count; ++i) {
            if (list->entries[i] != 0) {
                delete list->entries[i];
                list->entries[i] = 0;
            }
        }
        func_0019e08c(list->entries);
    }
    list->active = 0;
    list->kind = -1;
    list->first = 0;
    list->second = 0;
    list->selected = -1;
    list->count = 0;
    list->entries = 0;
    list->remaining = 0;
}
