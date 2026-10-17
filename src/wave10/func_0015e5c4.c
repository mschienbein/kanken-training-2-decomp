#include "game/screen.h"


extern void func_0015e5d4(struct DisplaySurface *surface, float x, float y, float z);

void func_0015e5c4(struct DisplayData *object, int index, float x, float y, float z)
{
    func_0015e5d4(object->parts[index]->surface, x, y, z);
}
