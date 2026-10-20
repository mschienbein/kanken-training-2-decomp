#include "game/item_type.h"

/* [INFERENCE] The table-only lifecycle entry has no direct callers. */

extern "C" W15iItemType *func_001c6ae4(W15iItemType *object)
{
    object->vtable = data_00223f18;
    return object;
}
