#include "game/screen.h"


void func_001963f8(struct DisplayObject *event)
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
