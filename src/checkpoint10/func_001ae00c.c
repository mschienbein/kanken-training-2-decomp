#include "game/resource_descriptors.h"
#include "game/playback_screen.h"
#include "game/screen.h"
#include "game/input.h"

struct W19SelectionPanel {
    struct WidgetController controller;
    struct WidgetSlot *slots[25];
    struct InputSnapshot *inputs[12];
    int selected, hovered, previous_hovered, value;
    unsigned reservedb0[2];
    int enabled;
};

struct W19ChoiceMap {
    int count;
    signed char values[];
};


extern void func_001adc0c(struct W19SelectionPanel *panel, int index);


static const struct W19ChoiceMap *choice_map(const struct W19SelectionPanel *panel)
{
    return bss_002470cc[panel->selected];
}

static int initial_press(const struct InputSnapshot *input)
{
    return input->active == 1 && input->previous_active == 0;
}

void func_001ae00c(struct W19SelectionPanel *panel)
{
    int i, count;
    const struct W19ChoiceMap *map;
    int previous;
    if (panel->selected == -1 || panel->enabled == 0 || panel->inputs[0] == 0)
        return;
    previous = panel->hovered;
    map = choice_map(panel);
    count = map->count;
    for (i = 0; i < count; ++i) {
        struct InputSnapshot *input = panel->inputs[i];
        /* ORIGINAL BUG: only the first region is checked for null before this loop. */
        func_00170418(input);
        if ((input->state & 4) == 0 && (input->state & 1) != 0) {
            if ((input->state & 2) != 0) {
                if (input->released ||
                    !func_001703c4(&input->rectangle, input->position.x, input->position.y))
                    input->state = 1;
            } else if (initial_press(input)) {
                if (!func_001703c4(&input->rectangle, input->position.x, input->position.y)) {
                    input->state = 1;
                } else {
                    input->state |= 2;
                    func_0015e31c(1, 0x01000015);
                    panel->hovered = i;
                    panel->value = map->values[i];
                    break;
                }
            }
        }
    }
    if (panel->hovered != previous) {
        func_001adc0c(panel, panel->selected);
        panel->previous_hovered = panel->hovered;
    }
}
