#include "game/screen.h"


void func_0019637c(struct BoundDisplayObject *event, struct BoundDisplayObject **head,
                   int primary)
{
    struct BoundDisplayObject *current = *head;
    struct BoundDisplayObject *previous = 0;

    while (current && current->base.position[2] < event->base.position[2]) {
        previous = current;
        if (primary)
            current = previous->next;
        else
            current = previous->next_secondary;
    }
    if (primary) {
        event->next = current;
        if (previous)
            previous->next = event;
    } else {
        event->next_secondary = current;
        if (previous)
            previous->next_secondary = event;
    }
    if (previous == 0)
        *head = event;
}
