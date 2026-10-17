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

static __forceinline int get_entry_value(const struct W10WidgetController *controller,
                                                     int index)
{
    const struct W10WidgetEntry *entry = controller->slots[index];
    int value = 0;
    if (entry != 0 && entry->object != 0 && (entry->flags & 0x10))
        value = entry->value;
    return value;
}

int func_0016e03c(const struct W10WidgetController *controller, int index)
{
    return get_entry_value(controller, index + 11);
}
