#include "game/screen.h"

/* [INFERENCE] Callers forward an unused integer and third scale component. */
extern void func_0015e480(struct DisplayPaneLink *link, int index, float x, float y, float z);

void func_0015e474(struct BoundDisplayObject *object, int index, float x, float y, float z)
{
    func_0015e480(object->bindings[0]->link, index, x, y, z);
}
