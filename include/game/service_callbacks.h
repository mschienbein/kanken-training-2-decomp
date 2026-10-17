#ifndef GAME_SERVICE_CALLBACKS_H
#define GAME_SERVICE_CALLBACKS_H

struct ServiceCallbackState;
struct ServiceCallbacks;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
typedef int (*ServicePollCallback)(void *context);

typedef void (*ServiceNotifyCallback)(void *context);

struct ServiceCallbacks {
    int (*request)(void *context, unsigned request, unsigned state);
    void (*message)(void *context, unsigned kind, void **payload);
    void (*receive)(void *context, unsigned kind, void *data, unsigned size, int result);
    ServiceNotifyCallback begin;
    ServiceNotifyCallback end;
    ServicePollCallback poll;
    ServiceNotifyCallback suspend;
    ServiceNotifyCallback resume;
    ServiceNotifyCallback stop;
    ServiceNotifyCallback pause;
    ServiceNotifyCallback complete;
    void (*reserved_callback)(void);
    ServiceNotifyCallback ready;
    unsigned reserved34[2];
    void (*shutdown)(void);
};

struct ServiceCallbackState {
    signed char status;
    unsigned char reserved01[0x0b];
    struct ServiceCallbacks callbacks;
    void *contexts[16];
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct ServiceCallbackState data_00242a6c;
#ifdef __cplusplus
}
#endif

#endif
