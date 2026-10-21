#include "game/transition_controller.h"

extern "C" void func_00173f9c(struct PairCollection *controller, int paused)
{
    controller->data.paused = paused;
    if (paused != 0)
        controller->data.elapsed = 0.0f;
}
