#include "game/service_runtime.h"

struct GlobalSoundSettings {
    unsigned int reserved00;
    float values[2];
};
struct GlobalSoundParameters {
    unsigned int dirty;
    struct GlobalSoundSettings settings;
};

struct GlobalCoordinatorView {
    unsigned char reserved00[0x10a4];
    struct GlobalSoundParameters *parameters[4];
    unsigned char reserved10b4[0x1322 - 0x10b4];
    unsigned short selected;
};

static __forceinline void set_volume(struct GlobalSoundSettings *settings,
    int index, float value)
{
    /* ORIGINAL BUG: this two-output array is indexed without validation. */
    settings->values[index] = value;
}

void func_00180d9c(struct ServiceCoordinator *coordinator, int index, float value)
{
    func_00182210();
    if (func_0018f018()) {
        struct GlobalCoordinatorView *state = (struct GlobalCoordinatorView *)coordinator;
        struct GlobalSoundParameters *parameters = state->parameters[state->selected];
        set_volume(&parameters->settings, index, value);
        if (index == 0)
            parameters->dirty |= 0x1000000;
        else if (index == 1)
            parameters->dirty |= 0x2000000;
    }
    func_00182204();
}
