#include "ctr/svc.h"

/* INTEGRATOR: differs from src/sdk2/func_00121da4.c and related IPC clients:
 * the native static handle object stores the same CtrHandle in its value field;
 * declarations and clients must expose/use that field consistently. */
/* [INFERENCE] The original one-word handle wrapper's class name is unrecovered. */
struct ServiceHandle00243794 {
    CtrHandle value;

    explicit ServiceHandle00243794(CtrHandle initial = 0) : value(initial) {}
};

ServiceHandle00243794 data_00243794;
