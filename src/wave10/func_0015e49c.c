#include "game/screen.h"


extern void func_0015e4a8(struct DisplaySurface *surface, float x, float y);

void func_0015e49c(struct DisplayData *object, float x, float y)
{
    func_0015e4a8(object->parts[0]->surface, x, y);
}
