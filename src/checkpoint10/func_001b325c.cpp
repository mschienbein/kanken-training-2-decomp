#include "game/scene_lifecycle.h"

/* [INFERENCE] Common resource constructor for its exact initializer-linked global;
 * no complete derived class or native historical name is asserted. */

ScreenResourceManager *func_001b325c(ScreenResourceManager *object)
{
    object = func_00174484(object, 0, 0, 0);
    object->vtable = &data_0022360c;
    return object;
}
