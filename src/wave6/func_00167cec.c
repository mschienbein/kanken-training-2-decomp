#include "game/radar.h"

struct RadarChart *func_00167cec(const struct RadarWidget *widget, int index)
{
    return widget->slots[index]->chart;
}
