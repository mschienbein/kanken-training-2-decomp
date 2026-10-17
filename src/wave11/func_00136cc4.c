#include "game/exp_heap.h"
void func_00136cc4(struct IntrusiveList *list, unsigned short link_offset)
{
    list->link_offset = link_offset;
    list->head = 0;
    list->tail = 0;
    list->count = 0;
}
