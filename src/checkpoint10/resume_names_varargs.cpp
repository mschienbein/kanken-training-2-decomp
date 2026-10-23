#include "game/property_descriptor.h"

void func_001b7d50(NameTable *table, int count, va_list arguments)
{
    if (count <= 0) {
        table->kind = 0;
        return;
    }
    table->kind = 1;
    table->names = static_cast<const char **>(
        func_00193110(count * sizeof(*table->names), data_002232d5));
    /* ORIGINAL BUG: allocation failure is not checked before filling the table. */
    for (int i = 0; i < count; ++i)
        table->names[i] = va_arg(arguments, const char *);
    table->count = count;
}
