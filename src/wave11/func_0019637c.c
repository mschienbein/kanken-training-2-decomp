#include "game/screen.h"

/* INTEGRATOR: consolidate with BoundDisplayObject in game/screen.h; this view
 * identifies the second sort link at 0xb4 and the embedded event's 0xd0 extent. */
struct ResourceEvent {
    struct DisplayObject base;
    struct DisplayBinding *bindings[5];
    int screen;
    int index;
    struct ResourceEvent *next;
    struct ResourceEvent *secondary_next;
    unsigned char reservedb8[8];
    unsigned char queued;
    unsigned char reservedc1[0xf];
};

void func_0019637c(struct ResourceEvent *event, struct ResourceEvent **head,
                   int primary)
{
    struct ResourceEvent *current = *head;
    struct ResourceEvent *previous = 0;

    while (current && current->base.position[2] < event->base.position[2]) {
        previous = current;
        if (primary)
            current = previous->next;
        else
            current = previous->secondary_next;
    }
    if (primary) {
        event->next = current;
        if (previous)
            previous->next = event;
    } else {
        event->secondary_next = current;
        if (previous)
            previous->secondary_next = event;
    }
    if (previous == 0)
        *head = event;
}
