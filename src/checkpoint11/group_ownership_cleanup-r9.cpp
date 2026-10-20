/* SPDX-License-Identifier: GPL-3.0-only
 * Group ownership phase adapted from pinned NintendoWare4Ctr
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified: retain the observed ownership phase without the absent member clear.
 */
#include "game/layout_resources.h"

/* [INFERENCE] Source-level ownership helper; no out-of-line identity is claimed. */
static void release_groups(nw::lyt::GroupContainer *container)
{
    nw::lyt::GroupList &groups = container->GetGroupList();
    for (nw::lyt::GroupList::Iterator it = groups.GetBeginIter(); it != groups.GetEndIter();) {
        nw::lyt::GroupList::Iterator current = it++;
        groups.Erase(current);
        if (!current->IsUserAllocated())
            nw::lyt::Layout::DeleteObj(&*current);
    }
}
extern "C" nw::lyt::GroupContainer *func_001cc1e4(nw::lyt::GroupContainer *container)
{
    release_groups(container);
    return container;
}