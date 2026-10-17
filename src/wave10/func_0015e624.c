#include "game/screen.h"


extern void func_0015e634(struct DisplaySurface *surface, float x, float y);

void func_0015e624(struct DisplayData *object, int index, float x, float y)
{
    func_0015e634(object->parts[index]->surface, x, y);
}
