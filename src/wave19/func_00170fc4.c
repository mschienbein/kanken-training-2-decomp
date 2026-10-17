#include "game/screen.h"

extern const struct NameTable *bss_00290e98[];

void func_00170fc4(struct DisplayHandle *const *handles, int index, int value)
{
    if (index < 10 && handles[index] != 0) {
        const struct NameTable *table = bss_00290e98[index];
        if (table->count > 0)
            func_0017a968(handles[index]->object, table, value, 1);
    }
}
