#include "game/screen.h"
#include "game/character_panel.h"

/* [INFERENCE] Private set-and-return-slot operation; historical helper identity is unknown. */
static int replace_indicator(CharacterPanel *panel, int value, int index)
{
    WidgetController *controller = &panel->controller;
    if ((unsigned)value >= 3) func_0016e7e0(controller, index, 0);
    func_001b5b60(controller, index, value);
    return index;
}

extern "C" void func_0016e798(CharacterPanel *panel, int value)
{
    int index = replace_indicator(panel, value, 10);
    func_0016e7e0(&panel->controller, index, 1);
}
