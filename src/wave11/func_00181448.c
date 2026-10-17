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

extern struct ResourceEvent *data_00242248;
extern struct ResourceEvent *data_0024224c;
extern struct ResourceEvent *data_00242250;
extern void func_0019637c(struct ResourceEvent *event, struct ResourceEvent **head,
                         int primary);

void func_00181448(struct ResourceEvent *event)
{
    if (event->queued)
        return;
    if (event->screen == 7) {
        func_0019637c(event, &data_00242248, 1);
        func_0019637c(event, &data_0024224c, 0);
    } else if (event->screen == 3) {
        func_0019637c(event, &data_00242248, 1);
    } else if (event->screen == 4) {
        func_0019637c(event, &data_0024224c, 0);
    } else {
        func_0019637c(event, &data_00242250, 0);
    }
    event->queued = 1;
}
