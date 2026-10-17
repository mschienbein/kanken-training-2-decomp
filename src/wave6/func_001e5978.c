#include "game/radar.h"

static __forceinline void setRadarScale(const struct RadarWidget *widget, float scale)
{
    if (widget) {
        struct RadarChart *chart = func_00167cec(widget, 0);
        if (chart)
            chart->scale.value = scale;
    }
    /* ORIGINAL BUG: a null widget is still forwarded to the renderer. */
    func_00167918(widget, 0, -1);
}

void func_001e5978(const struct RadarWidget *widget, int count)
{
    struct RadarChart *chart;
    if (!widget)
        return;
    chart = func_00167cec(widget, 0);
    if (chart)
        chart->count = count;
    setRadarScale(widget, 1.0f);
}
