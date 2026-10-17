#include "game/material.h"
#include "game/runtime.h"
#include "game/screen.h"



extern "C" void func_00181100(BoundDisplayObject *object, int resource, int replace)
{
    TextureDisplayBinding *binding =
        reinterpret_cast<TextureDisplayBinding *>(object->bindings[0]);
    binding->resource = resource;
    if (resource != -1) {
        func_0014c5c4(binding->pane, binding->material, resource, replace);
        return;
    }
    Material **slot = binding->material;
    DisplayPane *pane = binding->pane;
    if (*slot) {
        (*slot)->vtable->destroy_delete(*slot);
        Material *material = static_cast<Material *>(
            func_0019ffa4(sizeof(Material), data_002232d5));
        if (material)
            material = func_0017fe04(material);
        *slot = material;
        /* ORIGINAL BUG: allocation failure is not checked before this access. */
        material->flags = (material->flags & 0xfe) | 1;
        func_0017fda4(pane, *slot);
    }
    func_0017fd0c(func_0017fd68(binding->pane), 0);
    func_0017fc7c(func_0017fd68(binding->pane), 0);
}
