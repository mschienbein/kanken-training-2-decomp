#include <preinclude.h>
#include "game/layout_lifecycle.h"
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Common.h>
#include "game/runtime.h"


void *func_001cf3a4(MaterialPane *pane)
{
    pane->vtable = data_002242f0;
    if (pane->resource && !pane->resource->IsUserAllocated()) {
        nw::lyt::Layout::DeleteObj(pane->resource);
        pane->resource = 0;
    }
    pane->vertices.Free();
    return func_0016a998(pane);
}

void func_001cf338(MaterialPane *pane)
{
    func_001852f4(func_001cf3a4(pane));
}
