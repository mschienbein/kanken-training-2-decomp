#include <stdbool.h>

struct InputRectangle {
    float x, y, width, height;
};

struct InputPoint {
    float x, y;
};

struct InputSnapshot {
    bool active, released;
    unsigned char reserved02[2];
    struct InputPoint position;
    bool previous_active, previous_released;
    unsigned char reserved0e[2];
    struct InputPoint previous_position;
    bool tracking;
    unsigned char reserved19[3];
    struct InputPoint start_position;
    struct InputRectangle rectangle;
    unsigned int state;
};

struct InputMode {
    unsigned int value;
};

struct InputItem {
    unsigned char reserved00[0x0c];
    unsigned short flags;
    unsigned char reserved0e;
    unsigned char state;
    unsigned char reserved10[2];
    unsigned short counter;
};

struct InputGroup {
    unsigned int reserved00;
    int count;
    struct InputItem **items;
    struct InputSnapshot input;
    unsigned char reserved44[0x3c];
    unsigned int enabled;
    struct InputMode mode;
};

struct InputOwner {
    unsigned char reserved00[0x20];
    struct InputGroup *groups[];
};

extern void func_00170418(struct InputSnapshot *input);

void func_0016f930(const struct InputOwner *owner, int index)
{
    struct InputGroup *group = owner->groups[index];
    int i;
    if (group && group->count > 0) {
        group->enabled = 1;
        func_00170418(&group->input);
        for (i = 0; i < group->count; ++i) {
            struct InputItem *item = group->items[i];
            if (item) {
                item->state = 0;
                item->counter = 0;
                item->flags &= ~6;
            }
        }
    }
}
