#include "game/w11_control.h"

extern void func_001b0058(struct W11ControlScreen *screen);

/* [INFERENCE] The local settling helper's historical identity is unrecovered.
 * The grouped layout-switch form matched middleware but not game or sdk.
 * Its isolated form is confirmed below under middleware. This is a compiler
 * witness, not a claim of historical middleware ownership. */
static void short_delay(int limit)
{
    unsigned count = 0;
    while ((int)count < limit)
        ++count;
}

void func_001b0600(struct W11ControlScreen *screen)
{
    if (screen->active != 0) {
        screen->selection = -1;
        switch (screen->layout) {
        case 0:
            func_001b0058(screen);
            break;
        case 1:
            func_001b0188(screen);
            break;
        }
    }
    short_delay(10);
}
