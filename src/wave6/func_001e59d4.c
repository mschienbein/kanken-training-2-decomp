#include "game/radar.h"
extern void func_00191740(int count, const unsigned *handles);

void func_001e59d4(const struct RadarWidget *widget, int index)
{
    struct RadarChart *chart = func_00167cec(widget, index);
    /* ORIGINAL BUG: a missing chart is dereferenced rather than skipped. */
    if (chart->buffers[0]) {
        func_00191740(1, &chart->buffers[0]);
        chart->buffers[0] = 0;
    }
    if (chart->buffers[1]) {
        func_00191740(1, &chart->buffers[1]);
        chart->buffers[1] = 0;
    }
}
