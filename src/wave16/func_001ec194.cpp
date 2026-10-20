#include "game/mode_dispatch.h"



void func_001ec194(ModeDispatchObject *object)
{
    if (object->mode == 0)
        func_001ec890(object);
    else if (object->mode == 1)
        func_001ed698(object);
}
