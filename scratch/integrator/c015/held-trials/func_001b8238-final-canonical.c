#include "game/training_screen.h"
#include "game/category_progress.h"
#include "game/functions.h"

/* [INFERENCE] Shared option-application helper; both real option lists
 * pass their native setter and supported maximum. */
static void apply_choice(int value, void (*set_value)(int), int maximum)
{
    switch (value) {
    case 0: set_value(0); break;
    case 1: set_value(1); break;
    case 2: set_value(2); break;
    case 3:
        if (maximum == 3)
            set_value(3);
        break;
    }
}

void func_001b8238(struct TrainingScreen *screen)
{
    int temporary;
    switch (screen->states[screen->layer]) {
    case 12:
        func_0016d6dc(screen, 2);
        screen->states[screen->layer] = 13;
        screen->initialized = 0;
        break;
    case 13:
    {
        int value = func_00178f84(screen->controls.list, 4);
        if (value != -1) {
            int selection;
            apply_choice(value, func_0016fccc, 3);
            selection = func_001758d4();
            if (selection == 3) {
                func_0017433c(0, (int)func_0017adb4() * 15);
            } else if (func_00172994(0) != selection + 0x01000003) {
                switch (func_001758d4()) {
                case 0: func_0017abf8(0, 0x01000003); break;
                case 1: func_0017abf8(0, 0x01000004); break;
                case 2: func_0017abf8(0, 0x01000005); break;
                }
            }
        } else {
            value = func_00178f84(screen->controls.list, 5);
            if (value != -1)
                apply_choice(value, func_0016fca0, 2);
        }
        if (func_0017710c(screen->controls.animation, 3) == 0) {
            screen->states[screen->layer] = 14;
            screen->initialized = 0;
        }
        break;
    }
    case 14:
        func_0016d6dc(screen, 0);
        func_0016d600(screen, screen->mode, 0);
        temporary = screen->layer;
        screen->layer = screen->previous_layer;
        screen->previous_layer = temporary;
        break;
    }
}
