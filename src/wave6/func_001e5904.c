#include "game/radar.h"

static __forceinline void getRadarVertex(const struct RadarChart *chart, unsigned vertex, float *x, float *y)
{
    *x = 2.0f + chart->coordinates[vertex * 3] * 104.0f;
    *y = chart->coordinates[vertex * 3 + 1] * 104.0f;
}

void func_001e5904(const struct RadarWidget *widget, unsigned index, float *x, float *y)
{
    float px;
    float py;
    struct RadarChart *chart;
    if (!widget)
        return;
    chart = func_00167cec(widget, 0);
    px = 0.0f;
    py = 0.0f;
    if (chart)
        getRadarVertex(chart, index + 1, &px, &py);
    if (x)
        *x = px;
    if (y)
        *y = py;
}
