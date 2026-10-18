#include "game/screen.h"


void func_001eb190(struct W10DrawingInput *controller)
{
    func_001826fc(&controller->displays[0]->base, 0);
    func_00180344();
    func_00166d5c(controller, 0, 0.0f, 0.0f);
    controller->finished = 0;
}
