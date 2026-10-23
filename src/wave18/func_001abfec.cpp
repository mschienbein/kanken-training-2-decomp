#include "game/scene_lifecycle.h"



extern "C" void func_001abfec(W18SceneA *scene)
{
    if (func_00171e30() != 0 && func_00177124(7) == 0) {
        func_00177084();
        func_00181b58(5);
        func_00170f6c(scene->objects);
        func_00176e24(&scene->base);
        func_00176df4();
        func_00176dc8(scene->base.index);
    }
}
