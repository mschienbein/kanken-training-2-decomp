#include "game/training_screen.h"
extern void func_001b8238(struct TrainingScreen *controller);
extern void func_001b8424(struct TrainingScreen *controller);

void func_001b7fc0(struct TrainingScreen *controller)
{
    switch (controller->layer) {
    case 0: func_001b8908(controller); break;
    case 1: func_001b8424(controller); break;
    case 2: func_001b8238(controller); break;
    }
}
