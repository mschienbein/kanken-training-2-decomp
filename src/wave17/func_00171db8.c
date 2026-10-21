#include "game/animation.h"

void func_00171db8(struct MenuControlSet *owner, int index, int child_index, float x, float y)
{
    struct MenuGroup *animation = owner->groups[index];
    if (animation) {
        struct MenuControlItem *child = animation->items[child_index];
        if (child) {
            struct DisplayObject *object;
            child->position.x = x;
            child->position.y = y;
            object = child->object;
            object->position[0] = x;
            object->position[1] = y;
            switch (func_0017a9a0(object, 0)) {
            case 5:
                func_0015e684(object, 0,
                              object->position[0], object->position[1], object->position[2]);
                break;
            case 6:
                func_00181700(object,
                              object->position[0], object->position[1], object->position[2]);
                break;
            }
        }
    }
}
