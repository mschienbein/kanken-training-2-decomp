#include "game/transition_controller.h"

extern "C" void func_00172c04(struct PairCollection *controller, int mode)
{
    controller->data.paused = 0;
    controller->data.mode = mode;
    controller->data.active = 1;
    controller->data.elapsed = 0.0f;
    func_001b6788(controller, mode);
}
