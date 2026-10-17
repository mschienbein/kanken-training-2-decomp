#include "game/screen.h"

void func_001eaf84(struct W8DisplaySet *screen, int index, float y)
{
    struct W8DisplayHandle *slot = screen->handles[index];
    float x = y;
    if (slot != 0)
        x = slot->position.x;
    func_00166eb0(screen, index, x, y);
}
