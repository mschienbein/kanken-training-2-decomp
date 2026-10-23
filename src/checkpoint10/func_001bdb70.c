#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/scene_lifecycle.h"
#include "game/screen_transition.h"
#include "game/selection.h"




static __forceinline void set_mode(struct W6SelectionReset *screen, int mode)
{
    screen->modes[screen->stack_index] = mode;
}

void func_001bdb70(struct W6SelectionReset *screen)
{
    switch (screen->modes[screen->stack_index]) {
    case 3:
        func_00181b68(7, 1, 1);
        screen->previous_modes[screen->stack_index] = screen->modes[screen->stack_index];
        set_mode(screen, 4);
        break;
    case 4:
        if (func_00177124(7) == 0) {
            func_0016c55c(screen);
            func_00181b68(7, 0, 1);
            screen->previous_modes[screen->stack_index] = screen->modes[screen->stack_index];
            set_mode(screen, 5);
        }
        break;
    case 5: {
        int state = func_00177124(7);
        if (state == 0)
            screen->stack_index = state;
        break;
    }
    }
}
