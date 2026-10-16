#include "game/screen.h"

extern void func_00195900(struct Widget *widget, int index, const char *name, int mode);

void func_0017a968(struct Widget *widget, const struct NameTable *table, int index, int mode)
{
    if (table->kind == 1 && index >= 0 && index < table->count) {
        const char *name = table->names[index];
        if (name != 0)
            func_00195900(widget, 0, name, mode);
    }
}
