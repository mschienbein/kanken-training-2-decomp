#include "game/selection.h"

static __forceinline const struct GlyphRecord *record_at(const struct RecordSet *set, int index)
{
    if (set->records != 0)
        return &set->records[index];
    return 0;
}

int func_001678c0(const struct RecordSet *set, int index)
{
    if (set->records != 0)
        return record_at(set, index)->kind;
    return 0;
}
