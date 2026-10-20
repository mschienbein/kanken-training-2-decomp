#ifndef GAME_RUNTIME_HOOKS_H
#define GAME_RUNTIME_HOOKS_H

struct RuntimeGuard;
struct RuntimeFinalizer;
struct RuntimeGuardVTable;
struct RuntimeFinalizerVTable;



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


struct RuntimeGuard {
    const RuntimeGuardVTable *vtable;
};
struct RuntimeFinalizer {
    const RuntimeFinalizerVTable *vtable;
};

extern "C" {

extern const RuntimeGuardVTable data_00223d68;
RuntimeGuard *func_00215eb4(RuntimeGuard *object);

extern const RuntimeFinalizerVTable data_00223d80;
RuntimeFinalizer *func_00216130(RuntimeFinalizer *object);
}
#endif

#endif
