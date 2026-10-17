#include "game/functions.h"
#include "game/notification.h"





void func_0016fca0(int mode)
{
    struct W10DrawingInput *controller;
    bss_00280af0.payload.storage.fields.mode = mode;
    controller = func_001819f4();
    if (controller)
        func_001eb1cc(controller, bss_00280af0.payload.storage.fields.mode);
}
