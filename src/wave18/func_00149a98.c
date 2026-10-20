#include "game/functions.h"
#include "game/resource_runtime.h"



struct W10DrawingInput *func_00149a98(struct W10DrawingInput *controller)
{
    int i;
    for (i = 0; i < 3; ++i) {
        if (controller->displays[i]) {
            func_00183208(controller->displays[i]);
            controller->displays[i] = 0;
        }
    }
    for (i = 0; i < 2; ++i) {
        if (controller->identifiers[i] != -1) {
            func_00180314(controller->identifiers[i]);
            controller->identifiers[i] = -1;
        }
    }
    if (controller->notification_active == 1) {
        controller->field89a = func_0014c338();
        controller->notification_active = 0;
    }
    return controller;
}
