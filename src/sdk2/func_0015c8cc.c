#include "game/hwr_runtime.h"
void func_0015c8cc(void)
{
    struct HwrTree *state = &data_0024385c->tree;
    unsigned int maximum = 0;
    unsigned short index;
    for (index = 0; index != 0xffff; index = state->nodes[index].next) {
        if (state->nodes[index].weight > maximum)
            maximum = state->nodes[index].weight;
        if ((state->nodes[index].previous == 0xffff ||
             !state->nodes[state->nodes[index].previous].active) &&
            (state->nodes[index].next == 0xffff ||
             !state->nodes[state->nodes[index].next].active)) {
            if (state->nodes[index].weight < 3)
                state->nodes[index].marker = 12;
        }
    }
    if (maximum != 0) {
        int scale = 0xe00u / maximum;
        for (index = 0; index != 0xffff; index = state->nodes[index].next) {
            state->nodes[index].scaled_weight = (state->nodes[index].weight * scale) / 512;
        }
    }
}
