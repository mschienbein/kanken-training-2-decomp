/* [INFERENCE] Private accessor boundaries and historical TU membership are
 * reconstruction hypotheses. The fixed prefix precedes a signed-count names
 * header and its variable-length UTF-16 pointer vector. */
#include "game/text.h"

static const struct TextTable *text_names(const struct W8TextTable *table)
{
    return (const struct TextTable *)(table + 1);
}

static int text_count(const struct W8TextTable *table)
{
    return text_names(table)->count;
}

const unsigned short *func_00178f5c(const struct W8TextTable *table, int index)
{
    const struct TextTable *names;
    int count;
    if (index < 0) return 0;
    names = text_names(table);
    count = text_count(table);
    return index < count ? text_table_entries(names)[index] : 0;
}
