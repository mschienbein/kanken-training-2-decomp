#include "game/property_text.h"

void func_001267c0(struct AttributeCursor *cursor,
                   const struct AttributeOwner *owner, unsigned index)
{
    const struct AttributeTable *table = owner->table;
    /* ORIGINAL BUG: the inclusive bound accepts the one-past record index. */
    cursor->record = table && index <= table->record_count ? &table->records[index] : 0;
}
