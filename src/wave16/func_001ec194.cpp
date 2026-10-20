#include "game/mode_dispatch.h"



void CategoryScene::prepare()
{
    CategoryScene *object = this;
    if (object->active_layer == 0)
        func_001ec890(object);
    else if (object->active_layer == 1)
        func_001ed698(object);
}
