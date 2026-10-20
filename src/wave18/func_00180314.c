#include "game/functions.h"
#include "game/material.h"
#include "game/shader.h"


void func_00180314(int index)
{
    struct ResourceRecord *record = &((struct ResourceRecord *)bss_002d5ea0.buffer)[index];
    if (record->present) {
        record->present = 0;
        func_0018f0dc(1, record->info);
    }
}
