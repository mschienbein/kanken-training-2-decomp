#include "game/training_screen.h"

void func_001bbc1c(struct TrainingScreen *controller)
{
    switch (controller->layer) {
    case 0: func_001bc2a0(controller); break;
    case 1: func_001bbfc4(controller); break;
    case 2: func_001bbdd4(controller); break;
    }
}
