#include "game/input_device.h"
#include "game/pool_list.h"

/* [INFERENCE] Array-constructor callback; ARM constructors return their object. */
void *func_0011ee94(void *storage)
{
    struct PoolList *list = storage;
    list->sentinel.next = &list->sentinel;
    list->count = 0;
    list->sentinel.previous = &list->sentinel;
    return list;
}
