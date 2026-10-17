#include "game/input_device.h"
#include "game/pool_list.h"

/* [INFERENCE] Array-constructor callback; ARM constructors return their object. */
struct PoolList *func_00144970(struct PoolList *list)
{
    list->sentinel.next = &list->sentinel;
    list->count = 0;
    list->sentinel.previous = &list->sentinel;
    return list;
}
