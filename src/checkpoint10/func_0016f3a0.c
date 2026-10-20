#include "game/screen_transition.h"

static __forceinline void set_active(struct BoundDisplayObject *object, int active)
{
    switch (func_0017a9a0(&object->base, 0)) {
    case 3:
        func_00195a08(&object->base, 0, active);
        break;
    case 6:
        func_001826fc(&object->base, active);
        break;
    case 5:
        func_0015e948(&object->base, active);
        break;
    }
}

void func_0016f3a0(struct TransitionScene *scene, int index)
{
    struct ScreenAnimation *animation = &scene->animations[index];
    struct BoundDisplayObject *object = animation->objects[0];
    int i;
    if (object) {
        func_0017a968(&object->base, animation->table, 2, 0);
        set_active(object, 1);
        {
            struct BoundDisplayObject *first = object;
            func_0016f534(first, 0, -1.0f);
        }
        for (i = 1; i < 5; ++i) {
            object = animation->objects[i];
            if (object)
                set_active(object, 0);
        }
        animation->mode = 6;
    }
}
