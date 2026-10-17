#include "game/scene_lifecycle.h"
#include "game/cleanup.h"

/* [INFERENCE] The base constructor's three configuration words are integer
 * state fields; all three are zero in this static object's initializer. */
/* INTEGRATOR: move to include/game/controller_construction.h */
extern "C" {
BaseObject *func_00174484(BaseObject *object, int first, int second, int third);

BaseObject *func_001ef790(BaseObject *object);
}

BaseObject *func_001ef790(BaseObject *object)
{
    object = func_00174484(object, 0, 0, 0);
    object->vtable = &data_00223934;
    return object;
}
