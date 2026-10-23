#include "game/scene_lifecycle.h"



extern "C" void func_001ed820(W18SceneC *scene)
{
    if (func_00171e30() != 0 && func_00177124(7) == 0) {
        func_00177084();
        func_001c13f4(scene->objects);
        func_00181b58(11);
        func_00176e24(&scene->base);
        func_00176df4();
        func_00176dc8(scene->base.index);
    }
}
