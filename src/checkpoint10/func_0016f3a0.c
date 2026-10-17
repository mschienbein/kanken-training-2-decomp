#include "game/screen.h"

struct ScreenAnimation {
    int mode;
    unsigned char reserved04[0x1c];
    const struct NameTable *table;
    unsigned char reserved24[0x10];
    struct BoundDisplayObject *objects[5];
    unsigned char reserved48[0xc];
};
struct TransitionScene {
    unsigned char reserved00[0x18];
    struct ScreenAnimation animations[];
};
extern void func_0016f534(struct BoundDisplayObject *object, int index, float duration);

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

/* INTEGRATOR: differs from src/wave4/func_0016f39c.c: the writable screen
 * argument is a TransitionScene containing fixed-stride animation entries. */
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
