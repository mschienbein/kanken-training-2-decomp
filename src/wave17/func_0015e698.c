#include "game/screen.h"

void func_0015e698(struct DisplayPaneLink *link, float x, float y, float z)
{
    struct DisplayPane *pane = link->pane;
    pane->position[0] = x;
    pane->position[1] = y;
    pane->position[2] = z;
    pane->flags &= 0xcf;
}
