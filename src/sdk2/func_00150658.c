#include "game/object_manager.h"

struct OperationSettings {
    unsigned value;
    unsigned char mode;
    unsigned char reserved05[3];
    unsigned duration;
};
struct OperationTransform {
    float values[6];
};
struct OperationKey {
    unsigned first;
    unsigned second;
    unsigned char mode;
};
struct Operation {
    unsigned char reserved00[0x0c];
    unsigned char active;
    unsigned char running;
    unsigned char reserved0e;
    unsigned char stopping;
    unsigned char reserved10[0x34];
    struct RegistryEntry registry_entry;
    struct RegistryEntry manager_entry;
    unsigned char reserved5c[3];
    unsigned char key_mode;
    unsigned reserved60;
    unsigned key_first;
    unsigned key_second;
    struct OperationSettings settings;
    unsigned char reserved78[0x28];
    unsigned char state;
    unsigned char reserveda1;
    unsigned char linked;
    unsigned char reserveda3[9];
    struct OperationTransform transform;
};


void func_00150658(struct Operation *operation, const struct OperationSettings *settings,
                   const struct OperationKey *key)
{
    if (operation->active) {
        operation->stopping = 1;
        if (operation->linked) {
            func_0017e634(func_00185d1c(), &operation->manager_entry);
            operation->linked = 0;
        }
        if (operation->running)
            operation->running = 0;
    }
    operation->settings.value = settings->value;
    operation->settings.mode = settings->mode;
    operation->settings.duration = settings->duration;
    operation->key_first = key->first;
    operation->key_second = key->second;
    operation->key_mode = key->mode;
    operation->state = 3;
    operation->active = 1;
    func_0017e790(func_0017e7c4(), &operation->registry_entry);
    func_0017e640(func_00185d1c(), &operation->manager_entry);
    operation->linked = 1;
}
