#include "game/w11_control.h"
#include "game/screen.h"
#include "game/controller.h"



extern "C" void func_00162f0c(ScreenController *controller)
{
    controller->update();
    func_0017457c(controller->motions);
    func_0015fc3c(controller);
    if (controller->indicators && controller->animation)
        func_0017a164(controller->indicators, func_0017a210(controller->animation));
    /* [INFERENCE] callee identity: inlined by armlink. */
    func_001acc18(controller->resource);
    controller->finish();
}
