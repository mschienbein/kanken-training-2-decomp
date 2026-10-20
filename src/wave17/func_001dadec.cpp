#include "game/heap_objects.h"



LargeHeapObject *func_001dadec(LargeHeapObject *object)
{
    object->vtable = &data_00224bf8;
    object->field678 = 0;
    object->field67c = 0;
    return object;
}
