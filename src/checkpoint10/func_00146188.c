#include "game/input_runtime.h"

struct CompactServiceConfiguration {
    unsigned short flags, enabled, reserved04, mode;
    unsigned value;
    unsigned short coefficients[4];
};
struct ExpandedServiceConfiguration {
    unsigned short flags, enabled, reserved04, mode;
    unsigned values[5];
    unsigned short coefficients[12];
};
struct InputController {
    struct CompactServiceConfiguration compact[2];
    struct ExpandedServiceConfiguration expanded[2];
};
extern int func_00180fe0(struct ServiceCoordinator *state, int channel,
    const struct CompactServiceConfiguration *configuration);
extern int func_00180e74(struct ServiceCoordinator *state, int channel,
    const struct ExpandedServiceConfiguration *configuration);

void func_00146188(struct InputController *controller)
{
    int index;
    for (index = 0; index < 2; ++index) {
        signed char channel = (signed char)index;
        controller->compact[channel].flags = 0xffff;
        func_00180fe0(&bss_004b5284, channel, &controller->compact[channel]);
        controller->expanded[channel].flags = 0xffff;
        func_00180e74(&bss_004b5284, channel, &controller->expanded[channel]);
    }
}
