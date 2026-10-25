/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr contributors, pinned commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd (gfx_SceneNode.h,
 * lyt_Animation.cpp, lyt_Pane.cpp, lyt_Group.cpp).
 * Modified 2026-10-17: selected address-bound entries, shared natural helpers,
 * and the existing canonical ABI views. Historical TU membership is [INFERENCE].
 */
#include <preinclude.h>
#include <nw/lyt/lyt_Group.h>
#include <nw/lyt/lyt_Pane.h>
#include <nw/lyt/lyt_Layout.h>

namespace nw { namespace lyt {
inline void Group::Init()
{
    m_UserAllocated = false;
}

/* [INFERENCE] Private construction helper; the canonical PaneLink base is unchanged. */
struct InitializedPaneLink : PaneLink {
    explicit InitializedPaneLink(Pane *pane) { target = pane; }
};

static PaneLink *new_pane_link(Pane *pane);

inline void Group::AppendPane(Pane *pane)
{
    if (PaneLink *link = new_pane_link(pane)) {
        m_PaneLinkList.PushBack(link);
    }
}
struct GroupPaneNames {
    const char (*names)[ResourceNameStrMax];
    explicit GroupPaneNames(const res::Group *resource)
      : names(reinterpret_cast<const char (*)[ResourceNameStrMax]>(resource + 1)) {}
    const char *at(int index) const { return names[index]; }
};

Group::Group(const res::Group *resource, Pane *root)
{
    Init();
    /* ORIGINAL BUG: ut::strcpy advances before checking the next byte,
     * including when the resource name is initially empty. */
    ut::strcpy(m_Name, sizeof(m_Name), resource->name);
    GroupPaneNames names(resource);
    for (int index = 0; index < resource->paneNum; ++index) {
        if (Pane *pane = root->FindPaneByName(names.at(index), true))
            AppendPane(pane);
    }
}
static PaneLink *new_pane_link(Pane *pane)
{
    return Layout::NewObj<InitializedPaneLink>(pane);
}
} }

