/* SPDX-License-Identifier: GPL-3.0-only
 * Native intrusive-list operations use the pinned NintendoWare4Ctr headers.
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 */
#include <preinclude.h>
#include <stddef.h>
#include <nw/ut/ut_LinkList.h>
#include "game/object_manager.h"
#include "game/functions.h"
struct ResourceRecord { unsigned amount; unsigned char reserved04[0x4c]; PoolLink link; };
struct ResourceRecordList { unsigned reserved00; nw::ut::LinkList<ResourceRecord, offsetof(ResourceRecord, link)> records; };
/* INTEGRATOR: move this recovered list interface to game/object_manager.h. */
extern "C" { unsigned func_0014d7a0(ResourceRecordList *list); }
static unsigned resource_sum(ResourceRecordList *list)
{
    unsigned total = 0;
    for (nw::ut::LinkList<ResourceRecord, offsetof(ResourceRecord, link)>::Iterator i = list->records.GetBeginIter(); i != list->records.GetEndIter(); ++i) total += i->amount;
    return total;
}
unsigned func_0014d7a0(ResourceRecordList *list)
{
    func_00192bd8(&func_00185d1c()->lock);
    unsigned total = resource_sum(list);
    func_00195d74(&func_00185d1c()->lock);
    return total;
}
