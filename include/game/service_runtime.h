#ifndef GAME_SERVICE_RUNTIME_H
#define GAME_SERVICE_RUNTIME_H

#include "ctr/ipc.h"
struct AudioDspParameterBlock;
struct AudioDspReply;
struct SignalState;
struct StreamCriticalSection;
struct StreamMutex;
struct ResourceServiceEvent;
struct ChannelWorkspace;
struct ExtendedWorkspace;
struct AudioChannel;

#include <stdint.h>
#include "game/result.h"






#include "game/stream_manager.h"
#include "ctr/svc.h"
#include "game/synchronization.h"
#include "game/aux_effect.h"
typedef void (*LifecycleCallback)(void);

/* Callback pending and resource busy are distinct fields of this state object. */
struct ServiceHandle { unsigned handle; };
struct ResourceServiceState {
    unsigned char reserved00[2];
    unsigned char pending;
    unsigned char reserved03[5];
    const struct ServiceHandle *initialized;
    unsigned int busy;
    unsigned char lock_storage[8];
    CtrHandle handle;
};
struct InvalidHandleRecord { unsigned int reserved; CtrHandle handle; };
struct SharedPublicationState {
    signed char initialized;
    signed char published;
    unsigned char pending;
    unsigned char stopped;
    signed char shared_status;
    unsigned char ready;
    unsigned char reserved06[2];
    struct SignalState event;
};


struct WorkspacePair {
    struct ChannelWorkspace *channels;
    struct ExtendedWorkspace *extended;
};
struct ServiceOutput {
    unsigned char active;
    unsigned char enabled;
    unsigned char output_mode;
    unsigned char flag03;
    float first_gain;
    float volume;
    float channels[2];
    AuxCallback callbacks[2];
    void *contexts[2];
    unsigned char flag24, flag25;
    signed char shared_status;
    unsigned char reserved27;
    float scale28, scale2c;
    int consumed_samples;
    struct WorkspacePair workspaces[2];
    struct StreamMutex mutex;
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
    CtrHandle ready_event, completion_event;
    struct StreamMutex mutex;
    unsigned char reserved14[0x109c - 0x14];
    union {
        unsigned short *completion_slots[150];
        struct {
            unsigned short *reserved109c[2];
            struct AudioDspParameterBlock *voice_parameters[2];
            struct AudioDspReply *replies[2];
            struct AudioDspParameterBlock *auxiliary_parameters[2][24];
        } audio;
    } storage;
    struct ServiceBufferPair buffers[5];
    unsigned char enabled, pending;
    unsigned short serial, current_buffer, completion_slot;
    int expected;
    unsigned extra;
    unsigned char reserved132c[0x15b0 - 0x132c];
    unsigned char publish;
};
struct AudioChannel;
struct HandleManager {
    unsigned pending, queued, completed;
    unsigned short identifier;
    unsigned char active, reserved0f;
    struct StreamMutex mutex;
    unsigned char reserved1c[0x18dc - 0x1c];
    struct AudioChannel *channels[24];
};

struct SharedConfiguration {
    unsigned char reserved00[0xc0];
    signed char shared_status;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct ResourceServiceState data_00242afc;
extern LifecycleCallback bss_0049eec4[8];
extern void (*bss_0049ef04[8])(void);
extern struct ResourceServiceEvent bss_0049ef24;
extern struct StreamCriticalSection data_00242b0c;
extern CtrHandle data_00242b14;
extern const struct InvalidHandleRecord data_0021abd0;
extern struct SharedPublicationState data_00242d90;
extern struct ServiceOutput bss_004b5234;
extern struct ServiceCoordinator bss_004b5284;
extern struct HandleManager bss_004b6888;
extern void func_0012fbd4(struct ResourceServiceEvent *event);
extern void func_0012e620(struct StreamCriticalSection *lock);
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
extern void func_00148220(struct ServiceWorker *backend);
extern void func_0014b224(struct ServiceWorker *state, unsigned char value);
/* [INFERENCE] The wrapper forwards this word; the worker body does not read it. */
extern void func_0014fa48(struct ServiceWorker *backend, unsigned argument);
extern void func_001521dc(struct HandleManager *manager, struct AudioChannel *handle);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void func_00180558(const struct ServiceOutput *state, int index, void (**first)(void *, void *, void *), void **second);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
/* [INFERENCE] Native identities and roles; layouts follow matching consumers. */




extern "C" {
extern Result func_00145e84(uint16_t flags);
extern Result func_00145eb0(unsigned int *output);
extern Result func_00148014(uint16_t flags);
Result func_00145e08(unsigned short id, unsigned char *value);
Result func_00146008(unsigned short id, unsigned short *value);
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
extern void func_00195ea4(struct HandleManager *state);
extern void func_00146070(struct ServiceOutput *state);
#ifdef __cplusplus
}
#endif

#endif
