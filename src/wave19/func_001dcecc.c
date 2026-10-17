#include "game/packed_records.h"



const unsigned short *func_001dcecc(struct PackedRecordSet *list, int value)
{
    int index;
    int offset;
    if (list->records == 0)
        return 0;
    index = func_00169878(list, value);
    if (index < 0)
        return 0;
    offset = list->records[index].text_offset;
    if (offset != 0)
        return (const unsigned short *)((const unsigned char *)list->records + offset);
    return 0;
}
