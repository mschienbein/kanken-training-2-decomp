#define NW_RELEASE
#include "game/selection.h"
#include "game/character_panel.h"
#include "game/animation.h"
#include "game/notification.h"
#include "game/training_screen.h"

/* [INFERENCE] Address adjacency did not establish a shared class or TU with
 * the dictionary-search callback. This unit owns the option-application helper. */

static void play_music(int resource)
{
    func_0017abf8(0, resource);
}

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

extern "C" void func_001bbdd4(TrainingScreen *screen)
{
    switch (screen->states[screen->layer]) {
    case 11:
        func_0016c60c(screen, 2);
        screen->states[screen->layer] = 12;
        screen->initialized = 0;
        break;
    case 12:
    {
        int value = func_00178f84(screen->controls.list, 4);
        if (value != -1) {
            apply_choice(value, func_0016fccc, 3);
            int selection = func_001758d4();
            if (selection == 3) {
                func_0017433c(0, (int)func_0017adb4() * 60);
            } else if (func_00172994(0) != selection + 0x01000003) {
                switch (func_001758d4()) {
                case 0: play_music(0x01000003); break;
                case 1: play_music(0x01000004); break;
                case 2: play_music(0x01000005); break;
                }
            }
        } else {
            value = func_00178f84(screen->controls.list, 5);
            if (value != -1) {
                apply_choice(value, func_0016fca0, 2);
            }
        }
        if (func_0017710c(screen->controls.animation, 3) == 0) {
            screen->states[screen->layer] = 13;
            screen->initialized = 0;
        }
        break;
    }
    case 13:
        func_0016c60c(screen, 0);
        func_0016dae8(screen, screen->mode, 0);
        int temporary = screen->layer;
        screen->layer = screen->previous_layer;
        screen->previous_layer = temporary;
        break;
    }
}
