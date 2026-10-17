#include "game/cleanup.h"

/* [INFERENCE] The base constructor's three configuration words are integer
 * state fields; all three are zero in this static object's initializer. */
/* INTEGRATOR: move to include/game/controller_construction.h */
extern "C" {
BaseObject *func_00174484(BaseObject *object, int first, int second, int third);
extern const ConstructedVtable data_0022360c;
BaseObject *func_001b325c(BaseObject *object);
}

BaseObject *func_001b325c(BaseObject *object)
{
    object = func_00174484(object, 0, 0, 0);
    object->vtable = &data_0022360c;
    return object;
}
