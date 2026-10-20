#include "game/training_screen.h"

void TrainingScreenA::prepare()
{
    TrainingScreenA *controller = this;
    switch (controller->layer) {
    case 0: func_001b8908(controller); break;
    case 1: func_001b8424(controller); break;
    case 2: func_001b8238(controller); break;
    }
}
