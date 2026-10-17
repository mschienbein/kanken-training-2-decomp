#include "game/text_renderer.h"
#include "game/selection.h"

static __forceinline const struct GlyphRecord *record_at(const struct RecordSet *set, int index)
{
    if (set->records != 0)
        return &set->records[index];
    return 0;
}

int func_001e3490(const struct RecordSet *set, int index)
{
    if (set->records == 0)
        return -1;
    return (int)record_at(set, index)->reserved00[6];
}
