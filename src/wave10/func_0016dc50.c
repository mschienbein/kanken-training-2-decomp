#include "game/functions.h"
#include "game/character_panel.h"

int func_0016dc50(struct CharacterPanel *controller, int index)
{
    if (index >= controller->character_count)
        index = controller->character_count - 1;
    else if (index < controller->minimum)
        index = controller->minimum;
    func_001b5cc0(controller, 4, index);
    controller->selected = index;
    return index;
}
