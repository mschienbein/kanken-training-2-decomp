#ifndef GAME_SERVICE_CALLBACKS_H
#define GAME_SERVICE_CALLBACKS_H

struct ServiceCallbackState;
struct ServiceCallbacks;

#include "ctr/svc.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
typedef int (*ServicePollCallback)(void *context);

typedef void (*ServiceNotifyCallback)(void *context);

/* Both message-wait consumers forward all receive outputs and use an acceptance
 * result. Only its low byte is tested: int width/signedness are [INFERENCE],
 * not proof that historical callbacks produced a full-word decision. */
typedef int (*ServiceMessageCallback)(void *context, unsigned applet_id,
    int signal, void *buffer, unsigned capacity, unsigned size, CtrHandle handle);

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
    ServiceNotifyCallback reserved_callback;
    ServiceNotifyCallback ready;
    ServiceNotifyCallback callback13;
    unsigned reserved38;
    ServiceMessageCallback shutdown;
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
