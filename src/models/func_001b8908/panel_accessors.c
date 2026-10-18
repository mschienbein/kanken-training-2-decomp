#include "game/training_screen.h"
/* [INFERENCE] armlink-inlined character-panel accessors removed by --remove; identities inferred. */
void L034_set_input_enabled(struct CharacterPanel *panel, int enabled)
{
    panel->reserved84[8] = enabled;
}
int L034_get_selected(struct CharacterPanel *panel)
{
    return panel->selected;
}
