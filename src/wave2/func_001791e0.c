#include "game/text.h"

const unsigned short *func_001791e0(const struct TextTable *table, int index)
{
    if (index >= 0 && index < table->count)
        return text_table_entries(table)[index];
    return 0;
}
