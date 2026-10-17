#ifndef GAME_SYNCHRONIZATION_H
#define GAME_SYNCHRONIZATION_H

#include "game/stream_manager.h"

struct SignalState {
    unsigned int state;
    struct StreamCriticalSection critical;
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
#ifdef __cplusplus
}
#endif

#endif
