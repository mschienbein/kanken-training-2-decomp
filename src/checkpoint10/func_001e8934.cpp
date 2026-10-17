#include "game/screen.h"

/* INTEGRATOR: differs from include/game/selection.h: W6ListItem names only the halfword at +0x12;
 * this unit needs the full 0x14-byte choice record (object, table, id, flags, state, frame,
 * saved_frame, ticks). The group and controller layouts equal W6ListGroup/W6ListController. */
struct W8Choice {
    struct DisplayObject *object;
    const struct NameTable *table;
    unsigned short reserved08;
    short id;
    unsigned short flags;
    unsigned char reserved0e;
    unsigned char state;
    signed char frame;
    signed char saved_frame;
    unsigned short ticks;
};

struct W8ChoiceGroup {
    unsigned reserved00;
    int count;
    struct W8Choice **choices;
    unsigned char reserved0c[0x6c];
    int mode;
    int selected;
    int preserve_selected;
};

struct W8ChoiceController {
    unsigned char reserved00[0x20];
    struct W8ChoiceGroup *groups[21];
    unsigned char dirty;
};

extern "C" void func_001e8934(struct W8ChoiceController *controller, int index)
{
    struct W8ChoiceGroup *group = controller->groups[index];
    int i;
    if (group == 0)
        return;
    for (i = 0; i < group->count; ++i) {
        struct W8Choice *choice = group->choices[i];
        if (choice == 0)
            continue;
        if (group->preserve_selected != 0 && choice->id == group->selected)
            continue;
        choice->state = 0;
        choice->flags &= ~6;
        int frame = choice->frame;
        choice->saved_frame = frame;
        func_0017a968(choice->object, choice->table, frame, 1);
        choice->ticks = 0;
    }
    if (group->mode != -1)
        controller->dirty = 0;
    group->selected = -1;
    if (group->preserve_selected == 0)
        group->mode = -1;
}
