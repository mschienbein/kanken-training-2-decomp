#ifndef GAME_KERNEL_HANDLE_OWNER_H
#define GAME_KERNEL_HANDLE_OWNER_H

#include "ctr/svc.h"

/* The initializer and accepted close/clear entry agree on this one-word owner.
 * [INFERENCE] Native class spelling; the cleanup's receiver result is ignored
 * by destructor registration and is retained for its ordinary C entry. */
struct KernelHandleOwner {
    CtrHandle handle;
#ifdef __cplusplus
    KernelHandleOwner() : handle(0) {}
    ~KernelHandleOwner();
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct KernelHandleOwner data_00243030;
extern struct KernelHandleOwner *func_001c5294(struct KernelHandleOwner *object);
#ifdef __cplusplus
}
#endif

#endif
