#include "game/prefetch_stream.h"

struct BaseState {
    unsigned char reserved00[0x0c];
    unsigned char flag0c;
    unsigned char flag0d;
    unsigned char flag0e;
    unsigned char flag0f;
    unsigned char flag10;
    float scale_x;
    float scale_y;
    float value1c;
    float value20;
    float value24;
    float value28;
    unsigned char level;
    unsigned char reserved2d;
    unsigned char flag2e;
    float value30;
    unsigned char flag34;
    unsigned char flag35;
    float value38;
    float value3c;
    unsigned int value40;
};
extern void func_0016a068(struct BaseState *state);
extern void func_0017da3c(struct RecordState *state);
struct ResetChannel {
    unsigned char active;
    unsigned char reserved01[0x0b];
    unsigned char enabled, volume, pan;
    unsigned char reserved0f[9];
    float scale, value1c, value20;
};
struct CompositeState {
    struct BaseState base;
    unsigned char reserved44[0x0c];
    unsigned char flags[7];
    unsigned char reserved57;
    unsigned value58;
    unsigned char reserved5c[0x28];
    unsigned char flag84;
    unsigned char reserved85[0x77];
    float scale_x, scale_y, value104, value108;
    unsigned char reserved10c[0x110];
    unsigned value21c;
    unsigned char reserved220[8];
    struct StreamChannel groups[8];
    struct ResetChannel channels[4];
};

static void initialize_float_pair(float *first, float *second, float value)
{
    *first = value;
    *second = value;
}

void func_001d7780(struct CompositeState *state)
{
    unsigned i;
    func_0016a068(&state->base);
    state->flags[0] = 0;
    state->flags[1] = 0;
    state->flags[4] = 0;
    state->flags[5] = 0;
    state->flags[6] = 0;
    state->flags[2] = 0;
    state->flags[3] = 0;
    state->flag84 = 0;
    state->value58 = 0;
    state->value21c = 0;
    initialize_float_pair(&state->scale_x, &state->scale_y, 1.0f);
    initialize_float_pair(&state->value104, &state->value108, 0.0f);
    for (i = 0; i < 4; ++i) {
        struct ResetChannel *channel = &state->channels[i];
        channel->active = 0;
        channel->enabled = 0;
        channel->volume = 127;
        channel->pan = 64;
        channel->scale = 1.0f;
        channel->value1c = 0.0f;
        channel->value20 = 0.0f;
    }
    for (i = 0; i < 8; ++i) {
        struct StreamChannel *channel = &state->groups[i];
        unsigned j;
        channel->allocation = 0;
        channel->channel = 0;
        for (j = 0; j < 32; ++j)
            func_0017da3c(&channel->blocks[j]);
    }
}
