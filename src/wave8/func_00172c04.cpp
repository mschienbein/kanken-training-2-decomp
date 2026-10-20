#include "game/transition_controller.h"

extern "C" void func_00172c04(struct PairCollection *controller, int mode)
{
    controller->paused = 0;
    controller->mode = mode;
    controller->active = 1;
    controller->elapsed = 0.0f;
    func_001b6788(controller, mode);
}
