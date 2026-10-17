#include "game/layout.h"

void func_001c59fc(struct Pane *pane, unsigned alignment)
{
    pane->alignment = alignment;
    func_001c5a08(pane->style, alignment);
}
