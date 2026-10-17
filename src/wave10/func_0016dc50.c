#include "game/screen.h"

struct W10WidgetEntry {
    struct DisplayObject *object;
    unsigned char flags;
    unsigned char reserved05[3];
    unsigned short value;
};

struct W10WidgetController {
    unsigned char reserved00[0xc];
    struct W10WidgetEntry *slots[28];
    int count;
    int minimum;
    unsigned char reserved84[0xc];
    int selected;
};

extern void func_001b5cc0(struct W10WidgetController *controller, int slot, int row);

int func_0016dc50(struct W10WidgetController *controller, int index)
{
    if (index >= controller->count)
        index = controller->count - 1;
    else if (index < controller->minimum)
        index = controller->minimum;
    func_001b5cc0(controller, 4, index);
    controller->selected = index;
    return index;
}
