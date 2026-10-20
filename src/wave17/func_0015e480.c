#include "game/screen.h"

void func_0015e480(struct DisplayPaneLink *link, float x, float y)
{
    struct DisplayPane *pane = link->pane;
    pane->scale[0] = x;
    pane->scale[1] = y;
    pane->flags &= 0xcf;
}
