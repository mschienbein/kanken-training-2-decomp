#include "game/screen.h"
#include "game/display_depth.h"



void func_00181420(int screen, int index, float depth)
{
    struct BoundDisplayObject *event = func_00145b5c(screen, index);
    event->screen = screen;
    event->base.position[2] = depth;
    func_00181448(event);
}
