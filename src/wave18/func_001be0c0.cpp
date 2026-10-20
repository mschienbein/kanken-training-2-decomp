#include "game/scene_lifecycle.h"



extern "C" void func_001be0c0(W18SceneB *scene)
{
    if (func_00171e30() == 0)
        return;
    scene->base.vtable->prepare(&scene->base);
    func_001c20e8(scene->objects);
    scene->base.vtable->finish(&scene->base);
}
