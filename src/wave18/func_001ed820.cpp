#include "game/scene_lifecycle.h"



void CategoryScene::depart()
{
    CategoryScene *scene = this;
    if (func_00171e30() != 0 && func_00177124(7) == 0) {
        func_00177084();
        func_001c13f4(scene->frame);
        func_00181b58(11);
        func_00176e24(scene);
        func_00176df4();
        func_00176dc8(scene->index);
    }
}
