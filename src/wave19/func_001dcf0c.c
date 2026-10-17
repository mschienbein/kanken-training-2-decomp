#include "game/packed_records.h"

extern int func_00169878(struct PackedRecordSet *list, int value);

int func_001dcf0c(struct PackedRecordSet *list, int value)
{
    int index;
    if (list->records == 0)
        return 0;
    index = func_00169878(list, value);
    if (index < 0)
        return 0;
    return list->records[index].flag;
}
