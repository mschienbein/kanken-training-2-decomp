#include "game/material.h"
#include "game/screen.h"


void func_0014ab30(struct BoundDisplayObject *object, unsigned int value)
{
    struct Material *material = func_0017fd68(
        (struct PaneMaterialView *)object->bindings[0]->pane);
    func_0014d1ec(material)->value = value;
}
