#include <stdint.h>

#include "game/region_heap.h"

void func_00183724(struct FreeLink **head, void *storage, unsigned size)
{
    struct FreeLink *node = *head;
    uintptr_t start = (uintptr_t)storage;
    uintptr_t end = start + size;
    while (node) {
        if (start <= (uintptr_t)node && (uintptr_t)node < end)
            *head = node->next;
        else
            head = &node->next;
        node = node->next;
    }
}
