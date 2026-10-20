#include "game/animation.h"
#include "game/selection.h"

#include "game/menu_controller.h"

/* [INFERENCE] Internal assignment helper returns the stored stage. */
static int set_current_stage(struct InitializationMenuScene *screen, int stage)
{
    return screen->stages[screen->depth] = stage;
}

extern "C" void func_001a8048(struct InitializationMenuScene *screen)
{
    switch (screen->stages[screen->depth]) {
    case 13:
        func_00172c04(screen->transition, 0);
        func_001770cc(screen->controls, 13, 0);
        func_001770cc(screen->controls, 0, 0);
        func_001770cc(screen->controls, 1, 0);
        func_001770cc(screen->controls, 2, 0);
        func_001770cc(screen->controls, 3, 0);
        func_001770cc(screen->controls, 4, 0);
        func_001770cc(screen->controls, 5, 0);
        func_001770cc(screen->controls, 6, 0);
        func_001770cc(screen->controls, 7, 0);
        func_001770cc(screen->controls, 8, 0);
        func_001770cc(screen->controls, 9, 0);
        func_001770cc(screen->controls, 10, 0);
        func_001770cc(screen->controls, 11, 0);
        func_001770cc(screen->controls, 12, 0);
        set_current_stage(screen, 14);
        break;
    case 14:
        /* [INFERENCE] callee identity: active-field getter inlined by armlink
         * and removed after every reference was inlined. */
        if (transition_active(screen->transition) == 0) {
            func_00172c28(screen->transition);
            func_001770cc(screen->controls, 13, 1);
            switch (screen->stages[0]) {
            case 3:
                func_001770cc(screen->controls, 0, 1);
                break;
            case 4:
                func_001770cc(screen->controls,
                               func_001718ec(screen), 1);
                break;
            }
            screen->depth = 0;
        }
        break;
    }
}
