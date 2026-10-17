#include "game/runtime.h"
#include "game/heap_objects.h"



void func_001c5b68(LargeHeapObject *object)
{
    object->vtable = &data_00223d4c;
    object->field678 = 0;
    object->field67c = 0;
    func_001852f4(object);
}
