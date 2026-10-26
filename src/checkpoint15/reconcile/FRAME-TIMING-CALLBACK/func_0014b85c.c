#include "game/frame_timing.h"

static __inline void append_frame_sample(struct ObjectManager *state,
                                              const struct FrameTime *sample)
{
    if (state->count < state->capacity) {
        state->samples[state->count] = *sample;
        ++state->count;
    }
}

void func_0014b85c(void *context)
{
    struct ObjectManager *state = context;
    if (state->frame_sampling_enabled == 1) {
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
