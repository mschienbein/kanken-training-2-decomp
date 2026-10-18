#include "ctr/svc.h"

/* INTEGRATOR: differs from include/game/shared_memory.h: data_00242d8c is
 * the native handle object at SharedMemoryMapping's +4 handle slot; expose its
 * CtrHandle value field consistently in the mapping and standalone declaration. */
/* [INFERENCE] The original one-word handle wrapper's class name is unrecovered. */
struct SharedMappingHandle00242d8c {
    CtrHandle value;

    explicit SharedMappingHandle00242d8c(CtrHandle initial = 0) : value(initial) {}
};

SharedMappingHandle00242d8c data_00242d8c;
