/* Position slots retain a caller-provided Y coordinate when absent. */
#include "game/screen.h"

void func_00179130(struct W8DisplaySet *screen, int index, float x, float y)
{
    struct W8DisplayHandle *slot = screen->handles[index];
    if (slot != 0)
        y = slot->position.y;
    func_00166eb0(screen, index, x, y);
}
