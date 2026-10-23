#include "game/input.h"
#include "game/runtime.h"
#include "game/selection.h"

static void show_text(struct W6ListItem *item, int mode)
{
    func_0017a968(item->object, item->table, item->saved_frame, mode);
}

static void set_item_value(struct W6ListItem *item, int value, bool enabled)
{
    int mode;
    item->value = value;
    item->saved_frame = value;
    if (item->saved_frame >= item->table->count)
        item->saved_frame = item->table->count - 1;
    /* ORIGINAL BUG: negative indices are not checked before accessing names. */
    while (!item->table->names[item->saved_frame]) {
        ++item->saved_frame;
        if (item->saved_frame >= item->table->count) {
            item->saved_frame = 0;
            break;
        }
    }
    mode = 0;
    switch (item->label_index) {
    case 0x7f: case 0x80: case 0x81: case 0x86: case 0x87:
        mode = 1;
        break;
    }
    show_text(item, mode);
    if (value == 1 && !enabled && item->active == 1)
        item->state = 1;
}

void func_001e981c(void *screen, int group_index, int index, int value)
{
    struct W6ListController *owner = screen;
    struct W6ListGroup *group = owner->lists[group_index];
    struct W6ListItem *item;
    if (!group || index < 0)
        return;
    if (group->preserve_selected) {
        group->mode = index;
        group->selected = index;
    }
    item = group->items[index];
    if (!item)
        return;
    set_item_value(item, value, group->preserve_selected != 0);
}

