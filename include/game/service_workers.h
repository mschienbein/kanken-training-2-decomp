#ifndef GAME_SERVICE_WORKERS_H
#define GAME_SERVICE_WORKERS_H

struct ServiceWorkers;
struct ThreadHandle;
struct WaitableHandle;

#include "game/runtime.h"
#include "game/waitable_handle.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
struct ThreadHandle : WaitableHandle {
    __forceinline ~ThreadHandle();
};
#endif

struct ServiceWorkers {
    unsigned char reserved00[0xc];
    unsigned char main_running;
    unsigned char main_continue;
    unsigned char auxiliary_running;
    unsigned char auxiliary_continue;
    struct ThreadHandle main_thread;
    struct ThreadHandle auxiliary_thread;
    void *main_buffer;
    unsigned int reserved24;
    void *auxiliary_buffer;
    unsigned int reserved2c;
    unsigned char pending;
    unsigned char reserved31[0xb];
    int index;
    unsigned char reserved40[8];
    unsigned int event;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct ServiceWorkers *func_00148304(struct ServiceWorkers *workers);
#ifdef __cplusplus
}
#endif

#endif
