#include "game/functions.h"
#include "game/selection.h"
#include "game/character_panel.h"
#include "game/training_screen.h"


extern void func_0016e798(struct CharacterPanel *panel, int mode);

extern "C" void func_0016d600(struct TrainingScreenA *screen, int mode, int reset)
{
    screen->mode = mode;
    switch (mode) {
    case 0:
        func_0016e798(screen->characters, 2);
        func_00179724(screen->characters, 0, 0);
        screen->character_limit = 7;
        func_0016dc94(screen->drawing, 1);
        break;
    case 1:
        func_0016e798(screen->characters, 0);
        func_00179724(screen->characters, 1, -1);
        screen->character_limit = 1;
        func_0016dc94(screen->drawing, 0);
        break;
    }
    if (reset) {
        screen->selected = 0;
        func_0016dc50(screen->characters, screen->selected);
        func_00179ed4(screen->characters, 0);
    }
    func_001780c4(screen->controls, 2, 0, 0);
    func_001780c4(screen->controls, 2, 1, 0);
    func_0016dbc4(screen->drawing);
}
