#include "game/messages.h"


void func_00177b84(struct MessageLayout *layout, int enabled)
{
    if (enabled) {
        if (layout->activity_state == 2)
            layout->activity_state = 1;
    } else if (layout->activity_state == 1) {
        layout->activity_state = 2;
    }
    if (layout->activity_state != 0)
        func_00170dc0(layout, 2, enabled);
}
