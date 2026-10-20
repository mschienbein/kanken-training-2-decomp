#include "game/w11_control.h"
#include "game/screen.h"
#include "game/controller.h"



void ScreenController::update()
{
    ScreenController *controller = this;
    controller->prepare();
    func_0017457c(controller->controls);
    func_0015fc3c(controller);
    if (controller->indicators && controller->animation)
        func_0017a164(controller->indicators, func_0017a210(controller->animation));
    /* [INFERENCE] callee identity: inlined by armlink. */
    func_001acc18(controller->resource);
    controller->finish();
}
