#include "game/runtime_hooks.h"



RuntimeFinalizer *func_00216130(RuntimeFinalizer *object)
{
    object->vtable = &data_00223d80;
    return object;
}
