#include "game/animation.h"
#include "game/selection.h"

#include "game/transition_controller.h"

/* [INFERENCE] Internal assignment helper returns the stored stage. */
static int set_current_stage(struct W11jTransitionScreen *screen, int stage)
{
    return screen->stages[screen->depth] = stage;
}

void func_001a8048(struct W11jTransitionScreen *screen)
{
    switch (screen->stages[screen->depth]) {
    case 13:
        func_00172c04(screen->group, 0);
        func_001770cc(screen->animations, 13, 0);
        func_001770cc(screen->animations, 0, 0);
        func_001770cc(screen->animations, 1, 0);
        func_001770cc(screen->animations, 2, 0);
        func_001770cc(screen->animations, 3, 0);
        func_001770cc(screen->animations, 4, 0);
        func_001770cc(screen->animations, 5, 0);
        func_001770cc(screen->animations, 6, 0);
        func_001770cc(screen->animations, 7, 0);
        func_001770cc(screen->animations, 8, 0);
        func_001770cc(screen->animations, 9, 0);
        func_001770cc(screen->animations, 10, 0);
        func_001770cc(screen->animations, 11, 0);
        func_001770cc(screen->animations, 12, 0);
        set_current_stage(screen, 14);
        break;
    case 14:
        /* [INFERENCE] callee identity: active-field getter inlined by armlink
         * and removed after every reference was inlined. */
        if (transition_active(screen->group) == 0) {
            func_00172c28(screen->group);
            func_001770cc(screen->animations, 13, 1);
            switch (screen->stages[0]) {
            case 3:
                func_001770cc(screen->animations, 0, 1);
                break;
            case 4:
                func_001770cc(screen->animations,
                               func_001718ec((struct W6SelectionScreen *)screen), 1);
                break;
            }
            screen->depth = 0;
        }
        break;
    }
}
