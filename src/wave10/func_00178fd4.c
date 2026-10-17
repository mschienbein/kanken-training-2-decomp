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

extern int func_0016ec14(const struct InputMode *mode);

bool func_00178fd4(const struct InputOwner *owner, int index)
{
    const struct InputGroup *group = owner->groups[index];
    int result = 0;
    if (group) {
        if (func_0016ec14(&group->mode))
            return result;
        return 1;
    }
    return result;
}
