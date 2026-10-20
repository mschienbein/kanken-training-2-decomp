#include "game/scene_lifecycle.h"

/* [INFERENCE] Lifecycle names: this entry occupies the first vtable slot. */

ReturnScene::~ReturnScene()
{
    ReturnScene *scene = this;
    if (func_00177124(7) == 0) {
        func_00177084();
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->index);
    }
}
