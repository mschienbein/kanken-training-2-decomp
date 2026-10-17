/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_Bounding.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../ut/ut_RuntimeTypeInfo.h"

#include "lyt_Pane.h"

namespace nw{
namespace lyt{

struct ResBlockSet;
class DrawInfo;

class Bounding : public Pane
{
    typedef Pane Base;
public:
    NW_UT_RUNTIME_TYPEINFO;

    Bounding() {}
    Bounding(const res::Bounding* pBlock,const ResBlockSet& resBlockSet);
    virtual ~Bounding();
protected:
    virtual void DrawSelf(const DrawInfo& drawInfo);
};

}
} 

