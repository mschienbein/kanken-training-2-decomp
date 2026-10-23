/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; native Pane child-list and flag algorithms
 * from sources/libraries/lyt/lyt_Pane.cpp at pinned commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified 2026-10-20: recover the target byte-cursor traversal and virtual slot.
 * GNU GPL version 3; see src/nw4c/LICENSE.txt.
 */
#include <preinclude.h>
#include <nw/lyt/lyt_Pane.h>

namespace nw { namespace lyt {
/* [INFERENCE] DrawCommands is a descriptive native member name. The receiver
 * is the actual Pane owner proved by original vtable anchors, not a derived
 * view/downcast of children. No erased callee or callback cast is used. */
unsigned char* Pane::DrawCommands(unsigned char* cursor, LayoutDrawInfo* info,
                                 LayoutRenderContext* renderer)
{
    if (!(m_BasePosition & 0x10) || m_GlbAlpha)
        cursor = MakeUniformDataSelf(cursor, info, renderer);
    for (PaneList::Iterator it = m_ChildList.GetBeginIter();
         it != m_ChildList.GetEndIter(); ++it) {
        if (it->IsVisible())
            cursor = it->DrawCommands(cursor, info, renderer);
    }
    return cursor;
}
} }
