#include <preinclude.h>
#include "game/layout_lifecycle.h"
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Common.h>
#include "game/runtime.h"

struct MaterialPane {
    const void *const *vtable;
    unsigned char reserved04[0x138];
    nw::lyt::Material *resource;
    unsigned char reserved140[0x10];
    nw::lyt::internal::TexCoordAry vertices;
};

/* INTEGRATOR: move to include/game/layout_lifecycle.h. */
extern "C" {
extern const void *const data_002242f0[];
/* INTEGRATOR: differs from src/wave4/func_0016a994.c: the base destructor
 * returns its input object, as evidenced by deleting-destructor chaining. */

void *func_001cf3a4(MaterialPane *pane);
void func_001cf338(MaterialPane *pane);
}

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
