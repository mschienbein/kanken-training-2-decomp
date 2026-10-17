#ifndef GAME_SERVICE_RUNTIME_H
#define GAME_SERVICE_RUNTIME_H
#include "ctr/svc.h"
#include "game/synchronization.h"
typedef void (*LifecycleCallback)(void);
struct ResourceServiceLock;
struct ResourceServiceEvent;
struct ResourceServiceState {
    unsigned char reserved00[2];
    unsigned char pending;
    unsigned char reserved03[5];
    unsigned int initialized;
    unsigned int busy;
    unsigned char lock_storage[8];
    CtrHandle handle;
};
struct InvalidHandleRecord { unsigned int reserved; CtrHandle handle; };
struct SharedPublicationState {
    unsigned char initialized;
    unsigned char published;
    unsigned char pending;
    unsigned char stopped;
    signed char shared_status;
    unsigned char reserved05[3];
    struct SignalState event;
};
struct ServiceOutput {
    unsigned char active;
    unsigned char reserved01[0x25];
    signed char shared_status;
    unsigned char reserved27[0x25];
    int selected;
};
struct ServiceWorker {
    long long elapsed;
    unsigned char timed;
    unsigned char reserved09[3];
    unsigned char state, running;
    signed char publish;
    unsigned char service_running;
    unsigned char reserved10[0x10];
    void (*update)(void *);
    void *update_context;
    void (*service)(void *);
    void *service_context;
    unsigned char skip_service;
    unsigned char reserved31[3];
    struct StreamMutex mutex;
    struct SignalState request;
    struct SignalState completion;
};
struct ServiceBufferPair { unsigned int first, second; };
struct ServiceCoordinator {
    CtrHandle ready_event;
    CtrHandle completion_event;
    struct StreamMutex mutex;
    unsigned char reserved14[0x109c - 0x14];
    unsigned short *completion_slots[150];
    struct ServiceBufferPair buffers[5];
    unsigned char enabled, pending;
    unsigned short serial, current_buffer, completion_slot;
    int expected;
    unsigned int extra;
    unsigned char reserved132c[0x15b0 - 0x132c];
    unsigned char publish;
};
struct HandleManager {
    unsigned char reserved00[0x18];
    int selected;
};
struct ManagedHandle;
struct SharedConfiguration {
    unsigned char reserved00[0xc0];
    signed char shared_status;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct ResourceServiceState data_00242afc;
extern unsigned bss_0049eec4[8];
extern void (*bss_0049ef04[8])(void);
extern struct ResourceServiceEvent bss_0049ef24;
extern struct ResourceServiceLock data_00242b0c;
extern CtrHandle data_00242b14;
extern const struct InvalidHandleRecord data_0021abd0;
extern struct SharedPublicationState data_00242d90;
extern struct ServiceOutput bss_004b5234;
extern struct ServiceCoordinator bss_004b5284;
extern struct HandleManager bss_004b6888;
extern void func_0012fbd4(struct ResourceServiceEvent *event);
extern void func_0012e620(struct ResourceServiceLock *lock);
extern int func_0012e854(LifecycleCallback first, LifecycleCallback second,
    LifecycleCallback third);
extern void func_0013e84c(void);
extern void func_0013e9d8(void);
extern void func_0013e810(void);
extern int func_0012f1c0(struct ServiceOutput *state);
extern void func_0012f1b4(struct HandleManager *state);
extern void func_0013e894(struct ServiceCoordinator *worker, int preserve);
extern CtrResult func_00142838(struct ServiceCoordinator *state, int enabled);
extern CtrResult func_001821f4(struct ServiceOutput *thread, int status);
extern struct ServiceWorker *func_00182f88(void);
extern void func_00182210(void);
extern void func_00182204(void);
extern int func_0018f018(void);
extern void func_0017e7a0(struct ServiceOutput *output, int channel, unsigned value);
extern void func_0014fa0c(struct ServiceOutput *output, int channel, float value);
extern void func_001521dc(struct HandleManager *manager, struct ManagedHandle *handle);
#ifdef __cplusplus
}
#endif
#endif
