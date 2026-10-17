#include "game/runtime.h"
struct ResourceGroupObject;

extern void func_00186884(struct ResourceGroupObject *object);

void func_00186874(int screen, int index)
{
    func_00186884(func_00196cb8(screen, index));
}
