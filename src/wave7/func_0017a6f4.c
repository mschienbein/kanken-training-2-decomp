#include "game/screen.h"

extern void func_0017a9b4(struct SelectionList24 *screen, int value);
extern void func_0017a71c(struct SelectionList24 *screen, int index, int enabled);

void func_0017a6f4(struct SelectionList24 *screen, int selected)
{
    func_0017a9b4(screen, 0);
    func_0017a71c(screen, selected, 1);
}
