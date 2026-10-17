#include "game/screen.h"

extern void func_00170e58(struct Screen *screen, int index, int enabled);

void func_00170e50(struct Screen *screen, int index)
{
    func_00170e58(screen, index, 0);
}
