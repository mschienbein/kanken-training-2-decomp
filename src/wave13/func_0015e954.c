#include "game/screen.h"

void func_0015e954(struct DisplayPaneLink *link, int active)
{
    link->pane->flags = active | (link->pane->flags & 0xfe);
}
