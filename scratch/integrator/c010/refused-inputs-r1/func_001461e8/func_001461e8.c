#include "game/service_runtime.h"
struct ServiceCoordinatorConfig {
    unsigned char reserved00;
    unsigned char mode;
    unsigned char enabled;
    unsigned char option;
    float scale;
    float factor;
    float channels[2];
    void *allocations[2];
    unsigned char reserved1c[8];
    signed char formats[2];
    unsigned char reserved26;
    unsigned char count;
    unsigned short first;
    unsigned short second;
    unsigned int dirty;
    unsigned char active[2];
    unsigned short third;
};
extern void func_00180e14(struct ServiceCoordinator *state, float scale);
extern void func_00180d9c(struct ServiceCoordinator *state, signed char channel,
                         float value);
extern int func_00180d10(struct ServiceCoordinator *state, signed char channel,
                        unsigned short active);
extern int func_00180c84(struct ServiceCoordinator *state, signed char channel,
                        short format);
extern void func_00148988(struct ServiceCoordinator *state, unsigned char value);
extern void func_001488bc(struct ServiceCoordinator *state, unsigned char value);
extern void func_0014891c(struct ServiceCoordinator *state, unsigned short value);
extern int func_00148a9c(struct ServiceCoordinator *state, unsigned short value);
extern void func_00148854(struct ServiceCoordinator *state, unsigned short value);
extern void func_001489e8(struct ServiceCoordinator *state, unsigned short value);
extern void func_00148800(struct ServiceCoordinator *state, unsigned short value);

void func_001461e8(struct ServiceCoordinatorConfig *config)
{
    int index;
    func_00180e14(&bss_004b5284, config->scale * config->factor);
    for (index = 0; index < 2; ++index) {
        signed char channel = (signed char)index;
        func_00180d9c(&bss_004b5284, channel, config->channels[channel]);
        func_00180d10(&bss_004b5284, channel,
                      config->allocations[channel] || config->active[channel]);
        func_00180c84(&bss_004b5284, channel, config->formats[channel]);
    }
    func_00148988(&bss_004b5284, config->enabled);
    func_001488bc(&bss_004b5284, config->mode);
    func_0014891c(&bss_004b5284, config->first);
    func_00148a9c(&bss_004b5284, config->count);
    func_00148854(&bss_004b5284, config->second);
    func_001489e8(&bss_004b5284, config->third);
    config->dirty = 0;
    func_00148800(&bss_004b5284, config->option);
}
