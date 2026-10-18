#include "ctr/svc.h"

/* [INFERENCE] Original wrapper class name unrecovered.
 * INTEGRATOR: differs from include/game/service_state.h: the scalar handle
 * declaration exposes the first member only; this initializer constructs its
 * static-storage wrapper.  Share the type and reconcile the scalar view. */
struct StaticHandle {
    CtrHandle handle;
    StaticHandle() : handle(0) {}
};

StaticHandle data_00243040;
