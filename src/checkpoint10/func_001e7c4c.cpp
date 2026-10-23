#include "game/scene_lifecycle.h"

/* [INFERENCE] Common resource constructor for its exact initializer-linked global;
 * no complete derived class or native historical name is asserted. */

ScreenResourceManager *func_001e7c4c(ScreenResourceManager *object)
{
    object = func_00174484(object, 0, 0, 0);
    object->vtable = &data_00223838;
    return object;
}
