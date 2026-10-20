#include "game/service_runtime.h"

struct ChannelParameters {
    unsigned int dirty;
    unsigned char reserved04[0x34];
    unsigned char mode, option;
    short value3a, first, second;
    short coefficients[5];
    unsigned char reserved4a[0x56];
    unsigned char enabled, reserveda1;
    short count;
};

struct ChannelCoordinatorView {
    unsigned char reserved00[0x10b4];
    struct ChannelParameters *channels[2][24];
    unsigned char reserved1174[0x1aa];
    unsigned short current_buffer;
};

int func_0014ce14(struct ServiceCoordinator *coordinator, int channel,
    short first, short second)
{
    struct ChannelParameters *parameters;
    func_00182210();
    if (func_0018f018()) {
        struct ChannelCoordinatorView *state = (struct ChannelCoordinatorView *)coordinator;
        parameters = state->channels[state->current_buffer & 1][channel];
        parameters->first = first;
        parameters->second = second;
        parameters->dirty |= 0x800000;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}
