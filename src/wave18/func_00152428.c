#include "game/service_runtime.h"

struct ScalarSettings {
    unsigned char enabled;
    unsigned char reserved01[11];
    float values[];
};
extern void func_00180d9c(struct ServiceCoordinator *display, int index, float value);

void func_00152428(struct ScalarSettings *settings, int index, float value)
{
    if (settings->enabled) {
        settings->values[index] = value;
        func_00180d9c(&bss_004b5284, index, value);
    }
}
