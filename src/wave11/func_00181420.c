#include "game/screen.h"



void func_00181420(int screen, int index, float depth)
{
    struct DisplayObject *event = func_00145b5c(screen, index);
    event->screen = screen;
    event->position[2] = depth;
    func_00181448(event);
}
