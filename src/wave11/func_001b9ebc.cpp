#include "game/w11_control.h"
extern "C" void func_001b9ebc(W11AnimatedScreenA *screen)
{
    screen->vtable->begin_frame(screen);
    func_001b0600(screen->controls);
    if (screen->controls != 0 && screen->animation != 0)
        func_0016d554(screen->controls, func_0017a210(screen->animation));
    screen->vtable->end_frame(screen);
}
