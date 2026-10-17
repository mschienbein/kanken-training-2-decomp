#include "game/selection.h"
#include "game/character_panel.h"

struct W8EffectState;
struct W19TrainingScreen {
    unsigned char reserved00[0x14];
    struct W8EffectState *effects;
    unsigned char reserved18[0x34];
    struct W6ListController *choices;
    unsigned char reserved50[0x2c];
    struct CharacterPanel *panel;
    unsigned char reserved80[0x10];
    int state, previous_state;
    int phases[3];
    int phase_entered;
    unsigned char reserveda8[8];
    int count, layout, mode, edit_phase;
};

extern void func_0016e798(struct CharacterPanel *panel, int mode);
extern void func_0016dc94(struct W8EffectState *effects, int index);
extern int func_0016dc50(struct CharacterPanel *controller, int index);
extern int func_00179ed4(struct CharacterPanel *screen, int compact);
extern void func_0016dbc4(struct W8EffectState *effects);

void func_0016d600(struct W19TrainingScreen *screen, int mode, int reset)
{
    screen->mode = mode;
    switch (mode) {
    case 0:
        func_0016e798(screen->panel, 2);
        func_00179724(screen->panel, 0, 0);
        screen->layout = 7;
        func_0016dc94(screen->effects, 1);
        break;
    case 1:
        func_0016e798(screen->panel, 0);
        func_00179724(screen->panel, 1, -1);
        screen->layout = 1;
        func_0016dc94(screen->effects, 0);
        break;
    }
    if (reset) {
        screen->count = 0;
        func_0016dc50(screen->panel, screen->count);
        func_00179ed4(screen->panel, 0);
    }
    func_001780c4(screen->choices, 2, 0, 0);
    func_001780c4(screen->choices, 2, 1, 0);
    func_0016dbc4(screen->effects);
}
