#include "game/item_text.h"
const unsigned short *func_001eb694(const struct MenuItem *item, int index)
{
    if (index >= 0) {
        const struct TextTable *table = &item->alternate_text;
        if (index < table->count)
            return ((const unsigned short *const *)(table + 1))[index];
    }
    return 0;
}
