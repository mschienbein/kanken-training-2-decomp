#include "game/w11_control.h"
static void clear_slot(struct W11jGroupSlot *slot)
{
    if (slot != 0)
        slot->object = 0;
}

void func_001729bc(struct W11jWidgetGroup *group)
{
    int index = 0;
    do {
        clear_slot(group->slots[index]);
    } while (++index < 2);
}
