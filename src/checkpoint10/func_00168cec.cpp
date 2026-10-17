#include "game/screen.h"

/* INTEGRATOR: move to include/game/screen.h */
extern "C" {
void func_0016e300(WidgetController *controller, int index, unsigned char red,
                   unsigned char green, unsigned char blue, unsigned char alpha);
}

extern "C" void func_00168cec(WidgetController *controller, unsigned char alpha)
{
    func_0016e300(controller, 6, 255, 255, 255, alpha);
}
