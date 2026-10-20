#include "game/selection.h"

extern "C" int func_00178f84(struct MenuControlSet *controller, int index)
{
    int selected;
    if (controller->groups[index] == 0) {
        selected = -1;
    } else {
        selected = controller->groups[index]->selected;
        if (selected != -1)
            func_001e8934(controller, index);
    }
    return selected;
}
