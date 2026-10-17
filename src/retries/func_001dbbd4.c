#include "game/notification.h"
static __forceinline void advance_progress(struct NotificationPayload *payload, int mode)
{
    struct W10CourseProgress *state;
    if (mode != 2)
        state = &payload->storage.fields.primary;
    else
        state = &payload->storage.fields.secondary;
    switch (mode) {
    case 0:
        state->progress++;
        break;
    case 1:
        state->progress += 20;
        break;
    default:
        state->progress = data_002181b4[state->stage];
        break;
    }
    if (state->progress >= data_002181b4[state->stage]) {
        state->progress -= data_002181b4[state->stage];
        state->stage++;
        if (state->stage >= 5) {
            state->stage = 0;
            if (state->grade < 11)
                state->grade++;
        }
    }
}

void func_001dbbd4(int mode)
{
    advance_progress(&bss_00280af8, mode);
}
