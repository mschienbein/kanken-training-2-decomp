#include "game/screen.h"

struct CarouselScreen {
    struct WidgetController controller;
    struct WidgetSlot *slots[18];
    int position;
    int count;
    int selection;
};
extern void func_001b4ec8(struct CarouselScreen *screen, int index, int position);

void func_001791fc(struct CarouselScreen *screen, int enabled)
{
    struct WidgetSlot *slot = screen->slots[0];
    struct DisplayObject *object;
    if (slot == 0 || slot->object == 0)
        return;
    screen->position = 0;
    screen->selection = -1;
    func_001b4ec8(screen, 0, 0);
    object = slot->object;
    if (object == 0)
        return;
    {
        int type = func_0017a9a0(object, 0);
        if (type == 3)
            func_00195a08(object, 0, enabled);
        else if (type == 5)
            func_0015e948(object, enabled);
        else if (type == 6)
            func_001826fc(object, enabled);
    }
}
