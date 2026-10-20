#include "game/exp_heap.h"

void func_0012edd4(struct IntrusiveList *list, void *object)
{
    struct IntrusiveLink *link = (struct IntrusiveLink *)
        ((unsigned char *)object + list->link_offset);
    if (link->previous == 0) {
        list->head = link->next;
    } else {
        struct IntrusiveLink *previous = (struct IntrusiveLink *)
            ((unsigned char *)link->previous + list->link_offset);
        previous->next = link->next;
    }
    if (link->next != 0) {
        struct IntrusiveLink *next = (struct IntrusiveLink *)
            ((unsigned char *)link->next + list->link_offset);
        next->previous = link->previous;
    } else {
        list->tail = link->previous;
    }
    link->previous = 0;
    link->next = 0;
    --list->count;
}
