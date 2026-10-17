#include "game/selection.h"
#include <string.h>






void func_00190dc0(int category)
{
    struct W6Category *state = &bss_00290120[category];
    int i, j;
    state->identifier = (short)category;
    for (i = 0; i < 9; ++i) {
        state->patterns[i].selection = -1;
        state->patterns[i].value = 0;
        memset(&state->patterns[i].details.pattern, -1, sizeof(state->patterns[i].details.pattern));
    }
    for (j = 0; j < 12; ++j) {
        state->items[j].kind = -1;
        state->items[j].values[0][0] = 0;
        state->items[j].values[0][1] = 0;
        state->items[j].values[1][0] = 0;
        state->items[j].values[1][1] = 0;
    }
}
