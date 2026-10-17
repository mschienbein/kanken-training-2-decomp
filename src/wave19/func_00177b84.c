#include "game/screen.h"

struct WidgetActivity {
    struct DisplayHandle *handles[7];
    int state;
};
extern void func_00170dc0(struct DisplayHandle **handles, int index, int enabled);

void func_00177b84(struct WidgetActivity *widget, int enabled)
{
    if (enabled) {
        if (widget->state == 2)
            widget->state = 1;
    } else if (widget->state == 1) {
        widget->state = 2;
    }
    if (widget->state != 0)
        func_00170dc0(widget->handles, 2, enabled);
}
