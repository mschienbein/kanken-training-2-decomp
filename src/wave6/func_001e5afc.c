#include "game/radar.h"

void func_001e5afc(const struct RadarWidget *widget, int index, float value)
{
    struct RadarChart *chart;
    if (!widget)
        return;
    chart = func_00167cec(widget, 0);
    if (chart)
        chart->values[index] = value;
}
