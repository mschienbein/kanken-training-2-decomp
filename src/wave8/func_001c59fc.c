#include "game/layout.h"

extern void func_001c5a08(struct PaneStyle *style, unsigned alignment);

void func_001c59fc(struct Pane *pane, unsigned alignment)
{
    pane->alignment = alignment;
    func_001c5a08(pane->style, alignment);
}
