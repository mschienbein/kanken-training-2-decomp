#include "game/scene_lifecycle.h"



extern "C" void func_001acb04(W18SceneA *scene)
{
    scene->base.vtable->prepare(&scene->base);
    func_001c20e8(scene->objects);
    scene->base.vtable->finish(&scene->base);
}
