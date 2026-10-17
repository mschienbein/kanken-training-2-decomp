#include "game/radar.h"

void func_001705a8(const struct RadarWidget *widget, float red, float green, float blue, float alpha)
{
    struct RadarChart *chart;
    int index;
    if (!widget)
        return;
    chart = func_00167cec(widget, 0);
    if (chart)
        chart->alpha.value = alpha;
    /* ORIGINAL BUG: a missing chart is nevertheless dereferenced by this loop. */
    for (index = 0; index < 16; ++index) {
        chart->colors[index].red = red;
        chart->colors[index].green = green;
        chart->colors[index].blue = blue;
        chart->colors[index].alpha = chart->alpha;
    }
}
