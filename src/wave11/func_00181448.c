#include "game/screen.h"



/* [INFERENCE] The pass-through 00181420 entry establishes the one-object
 * contract. Two external sites also prepare zero in r1; that unused word is
 * not established as a required input. See the c007 caller-audit qualification. */
void func_00181448(struct DisplayObject *event)
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
