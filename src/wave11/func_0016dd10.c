#include "game/screen.h"

struct W11ControlScreen {
    unsigned char reserved00[0x0c];
    struct WidgetSlot *slots[10];
    int layout;
    int active;
    int selection;
};

extern void func_0016ff68(struct W11ControlScreen *screen, int index, int enabled);
extern void func_0016ff08(struct W11ControlScreen *screen, int index, float x, float y);

static __inline void set_all_controls(struct W11ControlScreen *screen, int enabled)
{
    int index;
    for (index = 0; index < 10; ++index)
        func_0016ff68(screen, index, enabled);
}

void func_0016dd10(struct W11ControlScreen *screen, int enabled)
{
    if (enabled == 0) {
        set_all_controls(screen, 0);
        screen->active = 0;
    } else {
        func_0016ff68(screen, 0, 1);
        func_0016ff68(screen, 1, 1);
        func_0016ff68(screen, 4, 1);
        if (screen->layout == 0) {
            func_0016ff68(screen, 2, 1);
            func_0016ff68(screen, 5, 1);
            func_0016ff08(screen, 5, 82.0f, 50.0f);
        } else if (screen->layout == 1) {
            func_0016ff68(screen, 3, 1);
            func_0016ff68(screen, 6, 1);
            func_0016ff08(screen, 6, 82.0f, 8.0f);
        }
        screen->active = 1;
    }
}
