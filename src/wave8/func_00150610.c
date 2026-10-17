#include "game/object_manager.h"

void func_00150610(struct Operation *operation, const struct OperationSettings *settings,
                   const struct OperationTransform *transform)
{
    operation->settings.value = settings->value;
    operation->settings.mode = settings->mode;
    operation->settings.duration = settings->duration;
    {
        float a = transform->values[0];
        float b = transform->values[1];
        float c = transform->values[2];
        float d = transform->values[3];
        float e = transform->values[4];
        float f = transform->values[5];
        operation->transform.values[0] = a;
        operation->transform.values[1] = b;
        operation->transform.values[2] = c;
        operation->transform.values[3] = d;
        operation->transform.values[4] = e;
        operation->transform.values[5] = f;
    }
    operation->state = 1;
    func_0017e640(func_00185d1c(), &operation->manager_entry);
    operation->linked = 1;
}
