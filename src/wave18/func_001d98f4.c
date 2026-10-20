#include "game/functions.h"
/* [INFERENCE] Per-channel modulation state; untouched prefix belongs to its configuration. */
struct AudioModulationState {
    unsigned char reserved00[0x10];
    unsigned elapsed;
    float value;
    float scale;
    unsigned char active;
    unsigned char pending;
    unsigned char reserved1e[2];
};
void func_001d98f4(struct AudioModulationState *state)
{
    state->value = 0.0f;
    state->scale = 1.0f;
    state->elapsed = 0;
    state->active = 0;
    state->pending = 0;
}
