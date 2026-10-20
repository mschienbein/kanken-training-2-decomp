#include "game/packed_records.h"

int func_00169878(struct PackedRecordSet *list, int value)
{
    struct PackedRecord *record = list->records;
    int index;
    if (record != 0) {
        if (list->count <= 0) {
            int count = 0;
            struct PackedRecord *cursor = record;
            while (cursor->text_offset != 0) {
                ++cursor;
                ++count;
            }
            list->count = count;
        }
        for (index = 0; index < list->count; ++index, ++record) {
            if (record->identifier == value)
                return index;
        }
    }
    return -1;
}
