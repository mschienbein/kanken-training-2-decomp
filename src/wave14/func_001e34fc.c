#include "game/text_renderer.h"
#include "game/selection.h"

const void *func_001e34fc(const struct RecordSet *set, int index)
{
    const struct GlyphRecord *records = set->records;
    unsigned offset;
    if (records == 0)
        return 0;
    offset = records[index].reserved00[3];
    if (offset == 0)
        return 0;
    return (const unsigned char *)records + offset;
}
