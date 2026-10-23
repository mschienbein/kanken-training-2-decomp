#include "game/messages.h"

extern int func_00170c20(struct TrainingWidget *widget, int mode,
                         const unsigned short *text, unsigned short *output, int reset);

static void set_alignment(struct DisplayObject *object, signed char horizontal,
                          signed char vertical)
{
    if (object == 0)
        return;
    {
        unsigned char type = func_0017a9a0(object, 0);
        switch (type) {
        case 6:
            func_00180358(object, horizontal, vertical);
            break;
        case 5:
            func_0015e780((struct TrainingWidget *)object, (unsigned char)(horizontal + 1));
            func_0015e824((struct TrainingWidget *)object, (unsigned char)(vertical + 1));
            break;
        }
    }
}

static void set_position(struct DisplayObject *object, float x, float y)
{
    struct DisplayObject *position;
    if (object == 0)
        return;
    position = object;
    position->position[0] = x;
    position->position[1] = y;
    switch (func_0017a9a0(object, 0)) {
    case 5:
        func_0015e684((struct BoundDisplayObject *)object, 0,
                      position->position[0], position->position[1], position->position[2]);
        break;
    case 6:
        func_00181700((struct BoundDisplayObject *)object,
                      position->position[0], position->position[1], position->position[2]);
        break;
    }
}

void func_001ad5a8(struct MessageLayout *layout, int index)
{
    struct MessageEntry *entry = layout->entries[index];
    if (entry == 0 || entry->object == 0)
        return;
    {
        int length = func_00170c20((struct TrainingWidget *)entry->object, 1,
                                   layout->text, entry->output, 1);
        entry->width = (entry->spacing + 4.0f) * length;
    }
    set_alignment(entry->object, 1, -1);
    entry->x = 100.0f;
    set_position(entry->object, entry->x + layout->x, entry->y + layout->y);
}
