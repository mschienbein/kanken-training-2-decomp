#include "game/runtime.h"
#include "game/input_device.h"



void func_001c5b68(InputStorageBase *object)
{
    object->vtable = &data_00223d4c;
    object->word678 = 0;
    object->word67c = 0;
    func_001852f4(object);
}
