#include "game/w11_control.h"

#include "game/transition_controller.h"
/* [INFERENCE] The signed transition-state getter may have been armlink-inlined
 * and removed; neither its historical identity nor original outlining is recovered. */

void func_001a00f8(struct W11jTransitionScreen *screen)
{
    switch (screen->stages[screen->depth]) {
    case 19:
        func_00172c04(screen->group, 0);
        func_001770cc(screen->animations, 0, 0);
        func_001770cc(screen->animations, 2, 0);
        func_001770cc(screen->animations, 3, 0);
        func_001770cc(screen->animations, 4, 0);
        func_001770cc(screen->animations, 5, 0);
        screen->stages[screen->depth] = 20;
        break;
    case 20:
        if (r4040_transition_active(screen->group) == 0) {
            func_00172c28(screen->group);
            func_001770cc(screen->animations, 0, 1);
            func_001770cc(screen->animations, 2, 1);
            func_001770cc(screen->animations, 3, 1);
            func_001770cc(screen->animations, 4, 1);
            func_001770cc(screen->animations, 5, 1);
            screen->depth = 0;
        }
        break;
    }
}
