#include "game/packed_records.h"



int func_0017749c(struct PackedRecordSet *list, int value)
{
    int index;
    if (list->records == 0)
        return -1;
    index = func_00169878(list, value);
    if (index < 0)
        return -1;
    return list->records[index].value;
}
