#include "game/scene_lifecycle.h"

/* [INFERENCE] Lifecycle names: this entry occupies the first vtable slot. */

extern "C" void func_001e7c70(W17ExtendedScene *scene)
{
    scene->base.vtable = &data_00223838;
    if (func_00177124(7) == 0) {
        func_00177084();
        func_00176e24(&scene->base);
        func_00176df4();
        func_00176dc8(scene->selection);
    }
    func_00174480(scene);
}
