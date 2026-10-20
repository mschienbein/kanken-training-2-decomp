#ifndef GAME_FRAME_TIMING_H
#define GAME_FRAME_TIMING_H
#include "game/synchronization.h"

/* The worker's signed tick count is copied as one eight-byte time value. */
struct FrameTime { long long ticks; };
struct AudioFrameCallbacks {
    struct StreamMutex mutex;
    struct PoolList listeners;
    struct PoolList late_listeners;
};
struct FrameTimingState {
    unsigned char reserved00[0x1b8];
    struct FrameTime *samples;
    int capacity;
    int count;
    struct AudioFrameCallbacks callbacks;
    unsigned char reserved1e8[0x0e];
    unsigned char enabled;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_0014cad4(struct FrameTime *elapsed);
extern void func_0014caf0(struct FrameTime *destination, const long long *source);
extern void func_0014ba3c(struct FrameTimingState *state, const struct FrameTime *elapsed);
extern void func_0014b8d8(struct FrameTimingState *state);
#ifdef __cplusplus
}
#endif
#endif
