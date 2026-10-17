#include "game/layout.h"

extern void func_001c5958(struct PaneStyle *style, unsigned mode);

void func_001c5950(struct Pane *pane, unsigned mode)
{
    func_001c5958(pane->style, mode);
}
