#include "game/character_panel.h"

void func_0016e61c(struct CharacterPanel *state, int selected, int character)
{
    int index;
    if (selected >= state->character_count) {
        selected = state->character_count - 1;
    } else if (selected < state->minimum) {
        selected = state->minimum;
    }
    index = selected + 11;
    state->selected = selected;
    func_0016e648(&state->controller, index, character);
}
