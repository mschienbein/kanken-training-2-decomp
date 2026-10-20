#ifndef GAME_SYNCHRONIZATION_H
#define GAME_SYNCHRONIZATION_H

struct StreamCriticalSection;


#include "game/stream_manager.h"

struct SynchronizationOwner {
    const void *vtable;
    struct SignalState synchronization;
};
struct SignalCallback {
    unsigned char reserved00[0x10];
    struct SignalState *state;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_0019ea6c(struct SignalState *state);
extern void func_00195d94(struct SignalState *state);
extern void func_0019ea64(struct SynchronizationOwner *object);
/* [INFERENCE] Signed exclusive-state word and scalar success result,
 * evidenced by the recursive-mutex consumer and the emitted definition. */
extern int func_0011359c(int *state);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
extern struct SignalState data_0024301c;
#ifdef __cplusplus
}
#endif



#endif
