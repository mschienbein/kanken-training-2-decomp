#include "game/item_text.h"
const unsigned short *func_001eb694(const struct W12cItemTextList *item, int index)
{
    if (index >= 0) {
        const struct W12cItemTextTable *table = &item->text;
        if (index < table->count)
            return ((const unsigned short *const *)(table + 1))[index];
    }
    return 0;
}
