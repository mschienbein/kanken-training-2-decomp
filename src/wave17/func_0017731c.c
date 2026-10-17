#include <stdint.h>

struct W17SecondaryState {
    unsigned int reserved00[2];
    unsigned int value;
    unsigned int active;
};

extern void func_0019d95c(uintptr_t resource, int index, int force);

int func_0017731c(struct W17SecondaryState *state)
{
    if (state->active) {
        func_0019d95c(15, 1, 0);
        state->active = 0;
    }
    state->value = 0;
    return 0;
}
