#include "game/drawing_screen.h"

void TrainingScreenB::prepare()
{
    TrainingScreenB *controller = this;
    switch (controller->mode) {
    case 0: func_001bc2a0(controller); break;
    case 1: func_001bbfc4(controller); break;
    case 2: func_001bbdd4(controller); break;
    }
}
