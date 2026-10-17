#include "game/service_runtime.h"

struct W20OutputValues {
    unsigned short flags;
    unsigned short mode;
    unsigned short reserved04;
    unsigned short count;
    unsigned words[5];
    unsigned short levels[12];
};
struct W20OutputState {
    unsigned char reserved00[0x28];
    struct W20OutputValues channels[];
};
extern int func_00180e74(struct ServiceCoordinator *coordinator, int channel,
    const struct W20OutputValues *values);

int func_0014dfa8(struct W20OutputState *state, int channel,
    const struct W20OutputValues *values)
{
    if (values->flags & 1)
        state->channels[channel].mode = values->mode;
    if (values->flags & 4) {
        state->channels[channel].count = values->count;
        state->channels[channel].levels[0] = values->levels[0];
        state->channels[channel].levels[1] = values->levels[1];
        state->channels[channel].levels[2] = values->levels[2];
        state->channels[channel].levels[3] = values->levels[3];
        state->channels[channel].levels[4] = values->levels[4];
        state->channels[channel].levels[5] = values->levels[5];
        state->channels[channel].levels[6] = values->levels[6];
        state->channels[channel].levels[7] = values->levels[7];
        state->channels[channel].levels[8] = values->levels[8];
        state->channels[channel].levels[9] = values->levels[9];
        state->channels[channel].levels[10] = values->levels[10];
        state->channels[channel].levels[11] = values->levels[11];
    }
    if (values->flags & 2) {
        state->channels[channel].words[0] = values->words[0];
        state->channels[channel].words[1] = values->words[1];
        state->channels[channel].words[2] = values->words[2];
        state->channels[channel].words[3] = values->words[3];
        state->channels[channel].words[4] = values->words[4];
    }
    return func_00180e74(&bss_004b5284, channel, values);
}
