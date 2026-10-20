#include "game/functions.h"
#include "game/character_panel.h"

static int has_character(struct CharacterPanel *screen, int index)
{
    struct WidgetSlot *slot = screen->slots[index];
    return slot != 0 && slot->object != 0 && (slot->state.flags & 0x10) != 0;
}

static unsigned short get_character(struct CharacterPanel *screen, int index)
{
    struct WidgetSlot *slot = screen->slots[index];
    if (slot != 0 && slot->object != 0 && (slot->state.flags & 0x10) != 0)
        return slot->text[0];
    return 0;
}

static void set_character(struct CharacterPanel *screen, int index, unsigned short character)
{
    if (screen->character_count <= index)
        index = screen->character_count - 1;
    else if (screen->minimum > index)
        index = screen->minimum;
    screen->selected = index;
    func_0016e648(&screen->controller, index + 11, character);
}

int func_00179ed4(struct CharacterPanel *screen, int compact)
{
    int selected;
    int i;
    if (compact != 0) {
        selected = screen->selected;
        if (has_character(screen, selected + 11)) {
selected_character:
            if (selected == 6) {
                func_0016e648(&screen->controller, 17, 0);
            } else if (selected != -1) {
                for (i = 0; i < 6; ++i) {
                    if (i >= selected) {
                        set_character(screen, i, get_character(screen, i + 12));
                        func_0016e648(&screen->controller, i + 12, 0);
                    }
                }
            }
            if (selected != -1)
                return selected;
        } else if (has_character(screen, selected + 10)) {
            --selected;
            goto selected_character;
        }
    } else {
        screen->selected = 0;
        for (i = 11; i <= 18; ++i)
            func_0016e648(&screen->controller, i, 0);
    }
    return 0;
}
