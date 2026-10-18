#include "ctr/svc.h"

/* INTEGRATOR: differs from src/sdk2/func_001382f8.c and related IPC clients:
 * the native static handle object stores the same CtrHandle in its value field;
 * declarations and clients must expose/use that field consistently. */

/* [INFERENCE] The original one-word handle wrapper's class name is unrecovered. */
struct ServiceHandle0024376c {
    CtrHandle value;

    explicit ServiceHandle0024376c(CtrHandle initial = 0) : value(initial) {}
};

ServiceHandle0024376c data_0024376c;
