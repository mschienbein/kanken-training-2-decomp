#include "game/functions.h"
#include "game/screen.h"

void func_001eb1cc(struct W10DrawingInput *controller, unsigned index)
{
    float scales[3] = { 0.5f, 1.0f, 1.5f };
    controller->scale = scales[index];
}
