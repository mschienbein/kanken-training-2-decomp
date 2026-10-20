#include "game/selection.h"

extern "C" void func_001e8934(struct W6ListController *controller, int index)
{
    struct W6ListGroup *group = controller->lists[index];
    int i;
    if (group == 0)
        return;
    for (i = 0; i < group->count; ++i) {
        struct W6ListItem *choice = group->items[i];
        if (choice == 0)
            continue;
        if (group->preserve_selected != 0 && choice->identifier == group->selected)
            continue;
        choice->countdown = 0;
        choice->flags &= ~6;
        int frame = choice->initial;
        choice->text_index = frame;
        func_0017a968(choice->object, choice->table, frame, 1);
        choice->value = 0;
    }
    if (group->mode != -1)
        controller->refreshing = 0;
    group->selected = -1;
    if (group->preserve_selected == 0)
        group->mode = -1;
}
