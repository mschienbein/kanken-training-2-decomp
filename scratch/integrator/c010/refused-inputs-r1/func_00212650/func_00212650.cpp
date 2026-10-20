#include "ctr/svc.h"

/* INTEGRATOR: move to include/game/kernel_handle_owner.h with the identical
 * handle field from src/sdk/func_001c5294.c. */
struct KernelHandleOwner {
    CtrHandle handle;
    KernelHandleOwner() : handle(0) {}
    /* [INFERENCE] Native class identity. The original initializer registers
     * func_001c5294, whose accepted body closes and clears this handle. */
    ~KernelHandleOwner();
};

/* INTEGRATOR: differs from include/game/service_state.h: the scalar declaration
 * covers the handle word; initializer/destructor evidence establishes an owner. */
KernelHandleOwner data_00243030;
