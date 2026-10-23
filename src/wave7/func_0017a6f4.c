#include "game/screen.h"

extern void func_0017a9b4(struct Screen *screen, int value);
extern void func_0017a71c(struct Screen *screen, int index, int enabled);

void func_0017a6f4(struct Screen *screen, int selected)
{
    func_0017a9b4(screen, 0);
    func_0017a71c(screen, selected, 1);
}
