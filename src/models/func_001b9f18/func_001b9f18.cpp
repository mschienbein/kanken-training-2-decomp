#include "game/training_screen.h"
/* [INFERENCE] The drawing-state getter may have been armlink-inlined and removed;
 * the exact caller does not recover the historical getter identity or outlining. */

static void set_state(TrainingScreen *screen, int state)
{
    screen->states[screen->layer] = state;
    screen->initialized = 0;
}

void func_001b9f18(struct TrainingScreen *screen)
{
    switch (func_0017710c(screen->controls.animation, 2)) {
    case 0:
        if (func_0016e03c(screen->characters, 0)) {
            func_0016dbc4(screen->drawing);
            func_0016d4f8(screen->drawing);
            set_state(screen, 5);
        }
        return;
    case 1:
        if (screen->drawing_state != 2) {
            screen->selected = func_00179ed4(screen->characters, 1);
            func_0016dc50(screen->characters, screen->selected);
        }
        screen->drawing_state = 0;
        break;
    case 2:
        func_001770cc(screen->controls.animation, 0, 0);
        func_001770cc(screen->controls.animation, 1, 0);
        func_001770cc(screen->controls.animation, 2, 0);
        func_001780c4(screen->controls.list, 2, 0, 0);
        func_001780c4(screen->controls.list, 2, 1, 0);
        set_state(screen, 1);
        func_0017433c(0, (int)func_0017adb4() * 60);
        func_00181b68(7, 1, 1);
        func_00177b10(reinterpret_cast<CallbackTarget *>(screen), data_002184b0);
        return;
    }
    switch (screen->drawing_state) {
    case 0:
        func_00178e98(screen->controls.list, 2);
        if (func_0016e03c(screen->characters, 0)) {
            func_001780c4(screen->controls.list, 2, 0, 1);
            func_001780c4(screen->controls.list, 2, 1, 1);
start_drawing:
            func_0016dbc4(screen->drawing);
            screen->drawing_state = 1;
        } else {
            func_001780c4(screen->controls.list, 2, 0, 0);
            func_001780c4(screen->controls.list, 2, 1, 0);
            goto start_drawing;
        }
        break;
    case 1:
        if (scene_drawing_state(screen->drawing) == 2) {
            screen->drawing_state = 2;
            func_001780c4(screen->controls.list, 2, 1, 1);
        }
        break;
    case 2:
        if (func_0016d2fc(screen->drawing)) {
            screen->character = func_0016d2dc(screen->drawing);
            if (screen->character != 0) {
                func_0016e61c(screen->characters, screen->selected, screen->character);
                ++screen->selected;
                if (screen->selected >= screen->character_limit)
                    --screen->selected;
                screen->selected = func_0016dc50(screen->characters, screen->selected);
            }
            screen->drawing_state = 0;
        }
        break;
    }
    func_0016d038(screen->drawing);
}
