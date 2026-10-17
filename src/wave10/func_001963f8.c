#include "game/screen.h"

struct ResourceEvent {
    struct DisplayObject base;
    struct DisplayBinding *bindings[5];
    int screen;
    int index;
    struct ResourceEvent *next;
    struct ResourceEvent *next_secondary;
    unsigned char reservedb8[8];
    unsigned char queued;
};

extern struct ResourceEvent *data_00242248;
extern struct ResourceEvent *data_0024224c;
extern struct ResourceEvent *data_00242250;
extern void func_0018695c(struct ResourceEvent *event, struct ResourceEvent **head, int primary);

void func_001963f8(struct ResourceEvent *event)
{
    if (event->queued) {
        if (event->screen == 7) {
            func_0018695c(event, &data_00242248, 1);
            func_0018695c(event, &data_0024224c, 0);
        } else if (event->screen == 3) {
            func_0018695c(event, &data_00242248, 1);
        } else if (event->screen == 4) {
            func_0018695c(event, &data_0024224c, 0);
        } else {
            func_0018695c(event, &data_00242250, 0);
        }
        event->queued = 0;
    }
}
