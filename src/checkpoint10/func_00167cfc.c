#include "game/screen.h"

extern void func_0015e5c4(struct DisplayData *object, int index, float x, float y, float z);
extern void func_001816c0(struct BoundDisplayObject *object, float x, float y, float z);

static __forceinline void apply_position(struct BoundDisplayObject *object)
{
    switch (func_0017a9a0(&object->base, 0)) {
    case 6:
        func_00181700(object, object->base.position[0], object->base.position[1], object->base.position[2]);
        break;
    case 5:
        func_0015e684(object, 0, object->base.position[0], object->base.position[1], object->base.position[2]);
        break;
    }
}

static __forceinline void apply_scale(struct BoundDisplayObject *object)
{
    switch (func_0017a9a0(&object->base, 0)) {
    case 6:
        func_001816e0(object, object->base.scale[0], object->base.scale[1]);
        break;
    case 5:
        func_0015e474(&object->base, 0, object->base.scale[0], object->base.scale[1], object->base.scale[2]);
        break;
    }
}

static __forceinline void apply_rotation(struct BoundDisplayObject *object)
{
    switch (func_0017a9a0(&object->base, 0)) {
    case 6:
        func_001816c0(object, object->base.rotation[0], object->base.rotation[1], object->base.rotation[2]);
        break;
    case 5:
        func_0015e5c4((struct DisplayData *)object, 0, object->base.rotation[0], object->base.rotation[1], object->base.rotation[2]);
        break;
    }
}

void func_00167cfc(struct BoundDisplayObject *object, float x, float y, float z,
                   float scale_x, float scale_y, float rotation_x, float rotation_y)
{
    object->base.position[0] = x;
    object->base.position[1] = y;
    object->base.position[2] = z;
    object->base.scale[0] = scale_x;
    object->base.scale[1] = scale_y;
    object->base.scale[2] = 1.0f;
    object->base.rotation[0] = rotation_x;
    object->base.rotation[1] = rotation_y;
    object->base.rotation[2] = 0.0f;
    apply_position(object);
    apply_scale(object);
    apply_rotation(object);
}
