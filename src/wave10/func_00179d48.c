#include "game/animation.h"

static __forceinline void set_element_active(struct MenuControlItem *element, int enabled)
{
    struct DisplayObject *object;
    if (element->flags & 8)
        enabled = 0;
    object = element->object;
    if (object == 0)
        return;
    switch (func_0017a9a0(object, 0)) {
    case 3:
        func_00195a08(object, 0, enabled);
        break;
    case 5:
        func_0015e948(object, enabled);
        break;
    case 6:
        func_001826fc(object, enabled);
        break;
    }
}

void func_00179d48(struct MenuControlSet *owner, int panel_index, int element_index, int enabled)
{
    struct MenuGroup *panel = owner->groups[panel_index];
    struct MenuControlItem *element;
    if (panel == 0 || panel->items == 0 || panel->items[element_index] == 0)
        return;
    element = panel->items[element_index];
    if (enabled)
        element->flags &= ~8u;
    else
        element->flags |= 8u;
    set_element_active(panel->items[element_index], enabled);
}
