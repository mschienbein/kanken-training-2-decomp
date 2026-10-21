#include "game/training_screen.h"
/* [INFERENCE] The input-enable and selection accessors may have been armlink-inlined
 * and removed; these supplied definitions do not recover their historical identities. */



static __forceinline void set_state(struct TrainingScreenA *screen, int state)
{
    screen->states[screen->layer] = state;
    screen->initialized = 0;
}

static __forceinline void change_layer(struct TrainingScreenA *screen, int layer, int state)
{
    int previous = screen->layer;
    screen->layer = layer;
    screen->states[layer] = state;
    screen->previous_layer = previous;
}

void func_001b8908(struct TrainingScreenA *screen)
{
    switch (screen->states[screen->layer]) {
    case 2:
        if (func_00177124(7) == 0) {
            func_0016d6dc(screen, 0);
            func_0016d600(screen, screen->mode, 1);
            func_00177520(screen->message);
            func_00177b84(screen->message, 0);
            func_0017a258(screen->animation, 100.0f, 260.0f);
            func_001770cc(screen->controls, 0, 0);
            func_001770cc(screen->controls, 1, 0);
            func_001770cc(screen->controls, 2, 0);
            func_00181b68(7, 0, 1);
            set_state(screen, 3);
        }
        break;
    case 3:
        if (func_00177124(7) == 0)
            set_state(screen, 4);
        break;
    case 4:
        if (screen->initialized == 0) {
            screen->initialized = 1;
            func_001770cc(screen->controls, 0, 1);
            func_001770cc(screen->controls, 1, 1);
            func_001770cc(screen->controls, 2, 1);
            screen->drawing_state = 0;
            L034_set_input_enabled(screen->characters, 1);
        }
        if (L034_get_selected(screen->characters) != screen->selected)
            screen->selected = L034_get_selected(screen->characters);
        if (func_0017710c(screen->controls, 1) != -1) {
            func_0016dbc4(screen->drawing);
            func_0016d4f8(screen->drawing);
            change_layer(screen, 2, 12);
        } else {
            switch (func_00178f84(screen->controls, 0)) {
            case 0: func_0016d600(screen, 0, 1); break;
            case 1: func_0016d600(screen, 1, 1); break;
            }
            func_001b9f18(screen);
        }
        break;
    case 5:
        L034_set_input_enabled(screen->characters, 0);
        func_001770cc(screen->controls, 0, 0);
        func_001770cc(screen->controls, 1, 0);
        func_001770cc(screen->controls, 2, 0);
        func_00181b68(7, 1, 1);
        change_layer(screen, 1, 6);
        break;
    }
}
