#include "game/radar.h"

extern void func_0018da44(int count, unsigned *handles);
extern void func_00193d64(unsigned target, unsigned handle);
extern void func_0018d8e4(unsigned target, unsigned size, const void *source, unsigned usage);

void func_001e5a20(const struct RadarWidget *widget, int index)
{
    struct RadarChart *chart = func_00167cec(widget, index);
    /* ORIGINAL BUG: a missing chart is not checked before its buffers are written. */
    func_0018da44(1, &chart->buffers[0]);
    func_00193d64(0x8892, chart->buffers[0]);
    func_0018d8e4(0x8892, 0x1d0, 0, 0x88e4);
    func_0018da44(1, &chart->buffers[1]);
    func_00193d64(0x8893, chart->buffers[1]);
    func_0018d8e4(0x8893, 0x22, 0, 0x88e4);
}
