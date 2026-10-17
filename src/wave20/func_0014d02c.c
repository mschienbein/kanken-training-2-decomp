#include "game/material.h"
#include "game/screen.h"

extern void func_0014e65c(struct DisplayVertexArray *vertices, unsigned index,
                         const struct DisplayTexCoords *coordinates);

void func_0014d02c(struct DisplayVertexPane *pane, unsigned index,
                   const struct DisplayTexCoords *coordinates)
{
    func_0014e65c(&pane->vertices, index, coordinates);
    if (pane->material != 0)
        pane->material->flags &= 0xfbu;
}
