#include "game/synchronization.h"

struct FrameTime { long long ticks; };
struct AudioFrameCallbacks {
    struct StreamMutex mutex;
    struct PoolList listeners;
    struct PoolList late_listeners;
};
/* [INFERENCE] Common audio-frame prefix, recovered from both frame paths. */
struct FrameTimingState {
    unsigned char reserved00[0x1b8];
    struct FrameTime *samples;
    int capacity;
    int count;
    struct AudioFrameCallbacks callbacks;
    unsigned char reserved1e8[0x0e];
    unsigned char enabled;
};
extern void func_0014cad4(struct FrameTime *elapsed);
extern void func_0014ba3c(struct FrameTimingState *state, const struct FrameTime *elapsed);
extern void func_0014b8d8(struct FrameTimingState *state);

static __inline void append_frame_sample(struct FrameTimingState *state,
                                              const struct FrameTime *sample)
{
    if (state->count < state->capacity) {
        state->samples[state->count] = *sample;
        ++state->count;
    }
}

void func_0014b85c(struct FrameTimingState *state)
{
    if (state->enabled == 1) {
        struct FrameTime elapsed;
        struct FrameTime current;
        elapsed.ticks = 0;
        func_0014cad4(&current);
        elapsed = current;
        append_frame_sample(state, &elapsed);
        func_0014ba3c(state, &elapsed);
    }
    func_0014b8d8(state);
}
