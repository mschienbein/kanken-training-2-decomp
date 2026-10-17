#include "game/radar.h"

void func_001678dc(const struct RadarWidget *widget, float scale)
{
    if (widget) {
        struct RadarChart *chart = func_00167cec(widget, 0);
        if (chart)
            chart->scale.value = scale;
    }
    /* ORIGINAL BUG: a null widget is still forwarded to the renderer. */
    func_00167918(widget, 0, -1);
}
