#include "game/functions.h"
#include "game/material.h"
/* ResourceArray is copied exactly from src/wave6/func_00104f90.c. */

struct ResourceRecordPayload {
    unsigned words[4];
};
struct ResourceRecord {
    unsigned active;
    unsigned reserved04[2];
    struct ResourceRecordPayload payload;
};
extern struct ResourceArray bss_002d5ea0;
extern void func_0018f0dc(unsigned operation, struct ResourceRecordPayload *payload);

void func_00180314(int index)
{
    struct ResourceRecord *record = &((struct ResourceRecord *)bss_002d5ea0.buffer)[index];
    if (record->active) {
        record->active = 0;
        func_0018f0dc(1, &record->payload);
    }
}
