#include "game/selection.h"

struct W7hRecordOwner {
    unsigned char reserved00[0x18];
    const struct RecordSet *records;
};


int func_001e5dcc(const struct W7hRecordOwner *owner, int identifier,
                  int capacity, int *counts)
{
    int kind_count = func_00171d24(identifier);
    int index;
    int record_count;
    if (kind_count > capacity)
        return -1;
    for (index = 0; index < capacity; ++index)
        counts[index] = 0;
    record_count = func_001698f4(identifier, -1);
    for (index = 0; index < record_count; ++index) {
        if (func_00169920(identifier, index, -1)) {
            int kind;
            for (kind = 0; kind < kind_count; ++kind) {
                int expected = func_00171c58(identifier, kind);
                if (func_001678c0(owner->records, index) == expected) {
                    ++counts[kind];
                    break;
                }
            }
        }
    }
    return kind_count;
}
