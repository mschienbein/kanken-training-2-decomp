#include "game/object_manager.h"



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
