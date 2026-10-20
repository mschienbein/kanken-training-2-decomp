#include "game/w11_control.h"

#include "game/menu_controller.h"
/* [INFERENCE] The signed transition-state getter may have been armlink-inlined
 * and removed; neither its historical identity nor original outlining is recovered. */

extern "C" void func_001a00f8(struct MenuExtentScene *screen)
{
    switch (screen->stages[screen->depth]) {
    case 19:
        func_00172c04(screen->transition, 0);
        func_001770cc(screen->controls, 0, 0);
        func_001770cc(screen->controls, 2, 0);
        func_001770cc(screen->controls, 3, 0);
        func_001770cc(screen->controls, 4, 0);
        func_001770cc(screen->controls, 5, 0);
        screen->stages[screen->depth] = 20;
        break;
    case 20:
        if (r4040_transition_active(screen->transition) == 0) {
            func_00172c28(screen->transition);
            func_001770cc(screen->controls, 0, 1);
            func_001770cc(screen->controls, 2, 1);
            func_001770cc(screen->controls, 3, 1);
            func_001770cc(screen->controls, 4, 1);
            func_001770cc(screen->controls, 5, 1);
            screen->depth = 0;
        }
        break;
    }
}
