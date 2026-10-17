#include "game/screen.h"

void func_0015e5d4(struct DisplayPaneLink *link, float x, float y, float z)
{
    struct DisplayPane *pane = link->pane;
    pane->rotation[0] = x;
    pane->rotation[1] = y;
    pane->rotation[2] = z;
    pane->flags &= 0xcf;
}
