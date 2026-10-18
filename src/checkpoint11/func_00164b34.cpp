#include <stdarg.h>
#include "game/resource_descriptors.h"

/* The matching caller 002095c0 establishes the canonical PaneIndexList
 * allocation and promoted variadic inputs. Historical class spelling remains
 * [INFERENCE]; no separate ByteSelectionList identity is introduced. */
static __forceinline void populate_bytes(PaneIndexList *list, int count,
                                         va_list arguments)
{
    if (count <= 0)
        return;
    {
        for (int i = 0; i < count; ++i)
            list->indices[i] = va_arg(arguments, int);
        list->count = count;
    }
}

PaneIndexList::PaneIndexList(int value_count, ...)
{
    va_list arguments;
    va_start(arguments, value_count);
    populate_bytes(this, value_count, arguments);
    va_end(arguments);
}
