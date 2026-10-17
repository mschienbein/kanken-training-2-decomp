#include "game/text_renderer.h"
#include "game/selection.h"

const unsigned short *func_001e3524(const struct RecordSet *set, int index)
{
    const struct GlyphRecord *records = set->records;
    unsigned offset;
    if (records == 0)
        return 0;
    offset = records[index].reserved00[0];
    if (offset == 0)
        return 0;
    return (const unsigned short *)((const unsigned char *)records + offset);
}
