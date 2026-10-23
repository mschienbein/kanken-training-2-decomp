#include <preinclude.h>
#include "game/material.h"
#include "game/runtime.h"
#include "game/display_native.h"

extern "C" void func_00181100(DisplayObject *object, int resource, int replace)
{
    PaneDisplayAttachment *binding = display_pane_attachment(object, 0);
    binding->resource = resource;
    if (resource != -1) {
        func_0014c5c4(binding->picture, binding->material, resource, replace);
        return;
    }
    nw::lyt::Material **slot = binding->material;
    nw::lyt::Picture *pane = binding->picture;
    if (*slot) {
        delete *slot;
        nw::lyt::Material *material = static_cast<nw::lyt::Material *>(
            func_0019ffa4(sizeof(nw::lyt::Material), data_002232d5));
        if (material)
            new (material) nw::lyt::Material;
        *slot = material;
        /* ORIGINAL BUG: allocation failure is not checked before this access. */
        material->SetUserAllocated();
        pane->SetMaterial(*slot);
    }
    binding->picture->GetMaterial()->SetTexMapNum(0);
    binding->picture->GetMaterial()->SetTexCoordGenNum(0);
}
