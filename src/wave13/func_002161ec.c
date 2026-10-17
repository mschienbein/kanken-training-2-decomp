#include "game/command_group.h"
#include "game/pool_list.h"

void func_002161ec(struct PoolList *list)
{
    list->sentinel.next = &list->sentinel;
    list->count = 0;
    list->sentinel.previous = &list->sentinel;
}
