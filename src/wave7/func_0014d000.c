#include "game/screen.h"

void func_0014d000(struct BoundDisplayObject *object,
                   const struct DisplayTexCoords *coordinates)
{
    struct DisplayBinding *binding = object->bindings[0];
    func_0014e504((struct DisplayVertexPane *)binding->pane, 1);
    func_0014d02c((struct DisplayVertexPane *)binding->pane, 0, coordinates);
}
