#include "game/layout.h"

extern void func_001c5978(struct PaneStyle *style, unsigned direction);

void func_001c5970(struct Pane *pane, unsigned direction)
{
    func_001c5978(pane->style, direction);
}
