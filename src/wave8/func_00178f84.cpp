#include "game/selection.h"

extern "C" int func_00178f84(struct W6ListController *controller, int index)
{
    int selected;
    if (controller->lists[index] == 0) {
        selected = -1;
    } else {
        selected = controller->lists[index]->selected;
        if (selected != -1)
            func_001e8934(controller, index);
    }
    return selected;
}
