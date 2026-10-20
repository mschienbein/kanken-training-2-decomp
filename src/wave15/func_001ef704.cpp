#include "game/timed_screen.h"



void func_001ef704(W15gTimedScreen *screen)
{
    screen->vtable->update(screen);
    func_001c1590(screen->delay);
    if (screen->active) {
        unsigned value, minutes, seconds;
        func_001dbf40(&value, &minutes, &seconds, 0);
        func_0017a034(screen->numbers, 1, value);
        func_0017a034(screen->numbers, 2, minutes);
        func_0017a034(screen->numbers, 3, seconds);
    }
    return screen->vtable->finish(screen);
}
