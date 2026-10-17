#include "game/character_panel.h"

static __forceinline void enable_control(struct CharacterPanel *screen, int index)
{
    struct WidgetSlot *slot;
    func_0016e7e0(&screen->controller, index, 1);
    slot = screen->slots[index];
    if (slot != 0 && slot->object != 0)
        slot->state.flags &= ~1u;
}

template <int Label>
static __forceinline void set_control_label(struct CharacterPanel *screen, int index)
{
    struct WidgetSlot *slot;
    func_0016e3e0(&screen->controller, index, Label);
    slot = screen->slots[index];
    if (slot != 0 && slot->object != 0)
        slot->state.flags &= ~1u;
}

void func_00179724(struct CharacterPanel *screen, int primary, int secondary)
{
    func_0016e7e0(&screen->controller, 0, 0);
    func_0016e7e0(&screen->controller, 1, 0);
    func_0016e7e0(&screen->controller, 2, 0);
    func_0016e7e0(&screen->controller, 3, 0);
    if (screen->slots[0] != 0 && screen->slots[0]->object != 0)
        screen->slots[0]->state.flags |= 1;
    if (screen->slots[1] != 0 && screen->slots[1]->object != 0)
        screen->slots[1]->state.flags |= 1;
    if (screen->slots[2] != 0 && screen->slots[2]->object != 0)
        screen->slots[2]->state.flags |= 1;
    if (screen->slots[3] != 0 && screen->slots[3]->object != 0)
        screen->slots[3]->state.flags |= 1;
    switch (primary) {
    case 0:
        enable_control(screen, 0);
        screen->character_count = 7;
        return;
    case 1:
        set_control_label<0>(screen, 1);
        screen->character_count = 1;
        break;
    case 2:
        set_control_label<1>(screen, 1);
        screen->character_count = 2;
        break;
    case 4:
        enable_control(screen, 0);
        screen->character_count = 4;
        return;
    }
    switch (secondary) {
    case 0:
        enable_control(screen, 2);
        break;
    case 1:
        enable_control(screen, 3);
        break;
    }
}
