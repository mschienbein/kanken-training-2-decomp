#include "game/property_descriptor.h"

static __forceinline void initialize_table(NameTable *table)
{
    table->kind = 0;
    table->vtable = &data_00223664;
    table->count = 0;
    table->names = 0;
}

static __forceinline void populate_names(NameTable *table, int count, va_list args)
{
    int i;
    if (count > 0) {
        table->kind = 1;
        table->names = static_cast<const char **>(func_00193110(count * sizeof(*table->names), data_002232d5));
        /* ORIGINAL BUG: allocation failure is not checked before the table is filled. */
        for (i = 0; i < count; ++i)
            table->names[i] = va_arg(args, const char *);
        table->count = count;
    }
}

extern "C" NameTable *func_00164d5c(NameTable *table, int category, int identifier,
                                    const char *name, int count, ...)
{
    va_list args;
    initialize_table(table);
    table->category = category;
    table->identifier = identifier;
    table->name = name;
    va_start(args, count);
    populate_names(table, count, args);
    va_end(args);
    return table;
}
