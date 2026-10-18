#include "game/selection.h"
#include "game/character_panel.h"
#include "game/animation.h"
#include "game/notification.h"

/* [INFERENCE] Address adjacency did not establish a shared class or TU with
 * the dictionary-search callback. This unit owns the option-application helper. */
struct W8EffectState;
struct W19TrainingScreen {
    unsigned char reserved00[0x14];
    W8EffectState *effects;
    unsigned char reserved18[0x34];
    W6ListController *choices;
    unsigned char reserved50[0x2c];
    CharacterPanel *panel;
    unsigned char reserved80[0x10];
    int state, previous_state;
    int phases[3];
    int phase_entered;
    unsigned char reserveda8[8];
    int count, layout, mode, edit_phase;
};

/* INTEGRATOR: move to include/game/training_screen.h; the W19TrainingScreen
 * layout and callback inputs retain the existing training-screen view. */
extern "C" {
int func_00172994(int channel);
int func_0017abf8(int channel, int sound);
void func_0017433c(int channel, int duration);
void func_0016fca0(int enabled);
void func_0016c60c(W19TrainingScreen *screen, int mode);
void func_0016dae8(W19TrainingScreen *screen, int mode, int reset);
}

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

extern "C" void func_001bbdd4(W19TrainingScreen *screen)
{
    switch (screen->phases[screen->state]) {
    case 11:
        func_0016c60c(screen, 2);
        screen->phases[screen->state] = 12;
        screen->phase_entered = 0;
        break;
    case 12:
    {
        int value = func_00178f84(screen->choices, 4);
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
            value = func_00178f84(screen->choices, 5);
            if (value != -1) {
                apply_choice(value, func_0016fca0, 2);
            }
        }
        if (func_0017710c((const AnimationOwner *)screen->choices, 3) == 0) {
            screen->phases[screen->state] = 13;
            screen->phase_entered = 0;
        }
        break;
    }
    case 13:
        func_0016c60c(screen, 0);
        func_0016dae8(screen, screen->mode, 0);
        int temporary = screen->state;
        screen->state = screen->previous_state;
        screen->previous_state = temporary;
        break;
    }
}
