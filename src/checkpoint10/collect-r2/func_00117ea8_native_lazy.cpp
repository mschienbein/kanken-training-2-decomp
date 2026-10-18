#include <preinclude.h>
#include <nw/lyt/lyt_Pane.h>
#include "game/layout.h"

struct PaneCommandVtable {
    unsigned char reserved00[0x68];
    unsigned char *(*draw)(LayoutPane *pane, unsigned char *cursor,
                          LayoutDrawInfo *info, LayoutRenderContext *renderer);
};
struct PaneCommandPrefix { const PaneCommandVtable *vtable; };

/* INTEGRATOR: share this traversal contract with game/layout.h if selected.
 * [INFERENCE] Descriptive derived-class and method identity. The inherited
 * child list, global-alpha and flag layout follow the canonical NW4C Pane.
 * The native method alias names its own definition, not an unproved callee. */
class PaneCommandTraversal : public nw::lyt::Pane {
public:
    unsigned char *DrawCommands(unsigned char *cursor, LayoutDrawInfo *info,
                                LayoutRenderContext *renderer);
};

unsigned char *PaneCommandTraversal::DrawCommands(unsigned char *cursor,
        LayoutDrawInfo *info, LayoutRenderContext *renderer)
{
    if (!(m_BasePosition & 0x10) || m_GlbAlpha) {
        const PaneCommandVtable *vtable =
            reinterpret_cast<PaneCommandPrefix *>(this)->vtable;
        cursor = vtable->draw(reinterpret_cast<LayoutPane *>(this), cursor, info, renderer);
    }
    for (nw::lyt::PaneList::Iterator it = m_ChildList.GetBeginIter();
         it != m_ChildList.GetEndIter(); ++it) {
        if (it->IsVisible())
            cursor = static_cast<PaneCommandTraversal *>(&*it)->DrawCommands(cursor, info, renderer);
    }
    return cursor;
}
