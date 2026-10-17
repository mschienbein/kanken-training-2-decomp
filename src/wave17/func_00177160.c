#include <stdint.h>

struct W17PrimaryState {
    unsigned int reserved00[3];
    unsigned int value;
    unsigned int active;
};

extern void func_0019d95c(uintptr_t resource, int index, int force);

int func_00177160(struct W17PrimaryState *state)
{
    if (state->active) {
        func_0019d95c(15, 0, 0);
        state->active = 0;
    }
    state->value = 0;
    return 0;
}
