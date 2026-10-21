#include "game/scene_lifecycle.h"



MenuResetScene::~MenuResetScene()
{
    MenuResetScene *scene = this;
    if (func_00171e30() != 0 && func_00177124(7) == 0) {
        func_00177084();
        func_00181b58(5);
        func_00170f6c(scene->objects);
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->index);
    }
}
