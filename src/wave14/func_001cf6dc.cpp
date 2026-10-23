#include "game/layout_draw.h"
#include "game/matrix.h"
#include "game/screen.h"



#include "game/functions.h"
extern "C" void func_001cf6dc(LayoutDrawPane *pane, LayoutRenderContext *context)
{
    LayoutDrawState state;
    state.base.geometry = pane->geometry;
    func_001cf504(pane, &state);
    if (pane->pending) {
        func_001da0e0(&state);
        func_00216670(&state, pane->source, pane->source_id);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00125b54(&state);
        pane->pending = 0;
    }
    LayoutGeometryData *geometry = pane->geometry;
    if (!geometry->active && context) {
        if ((geometry->flags & 8) && geometry->external) {
            if (geometry->current == geometry->alternate)
                geometry->current = geometry->external;
            else
                geometry->current = geometry->alternate;
        }
        func_001da51c(context, &state);
    }
    func_00125b50(&state);
}
