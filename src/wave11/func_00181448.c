#include "game/screen.h"



void func_00181448(struct BoundDisplayObject *event)
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
