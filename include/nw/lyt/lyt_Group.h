/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_Group.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <cstddef>
#include "../ut/ut_LinkList.h"

#include "lyt_Pane.h"
#include "lyt_Types.h"

namespace nw{
namespace lyt{

struct PaneLink
{
    ut::LinkListNode link;
    Pane* target;
};

typedef ut::LinkList<PaneLink, offsetof(PaneLink, link)> PaneLinkList;

class Group
{
public:
    Group();
    Group(const res::Group* pResGroup, Pane* pRootPane);
    virtual ~Group();
    void AppendPane(Pane* pPane);

    const char* GetName() const {return m_Name;}
    PaneLinkList& GetPaneList() {return m_PaneLinkList;}
    bool IsUserAllocated() const {return m_UserAllocated != 0;}
    void SetUserAllocated() {m_UserAllocated = 1;}
    ut::LinkListNode m_Link_;

protected:
    void Init();

    PaneLinkList m_PaneLinkList;
    char m_Name[ResourceNameStrMax + 1];
    u8 m_UserAllocated;
    u8 m_Padding[2];
};

typedef ut::LinkList<Group, offsetof(Group, m_Link_)> GroupList;

class GroupContainer
{
public:
    GroupContainer() {}
    ~GroupContainer();
    void AppendGroup(Group* pGroup);
    Group* FindGroupByName(const char* findName);

    GroupList& GetGroupList() {return m_GroupList;}
protected:
    GroupList m_GroupList;
};

}
}

