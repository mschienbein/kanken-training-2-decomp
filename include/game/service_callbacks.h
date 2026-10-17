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

/* The poll consumer walks this context-taking notification list at state +4. */
struct ServiceCallbackNode {
    struct ServiceCallbackNode *previous;
    struct ServiceCallbackNode *next;
    ServiceNotifyCallback callback;
    void *context;
};

struct ServiceCallbacks {
    int (*request)(void *context, int pending, unsigned char mode);
    void (*message)(void *context, unsigned short command, CtrHandle *reply_handle);
    void (*receive)(void *context, unsigned applet_id, const void *data,
                    unsigned size, CtrHandle handle);
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
    /* func_00115000 invokes both slots; the second is an indirect tail call. */
    ServiceNotifyCallback before;
    ServiceNotifyCallback after;
    ServiceMessageCallback shutdown;
};

struct ServiceCallbackState {
    signed char status;
    unsigned char reserved01[3];
    struct ServiceCallbackNode *notifications;
    unsigned reserved08;
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
