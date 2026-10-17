/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/res/gfx_ResParticleShape.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../../ut/ut_ResUtil.h"
#include "../../ut/ut_ResDictionary.h"
#include "gfx_ResTypeInfo.h"
#include "gfx_ResVertex.h"
#include "gfx_ResShape.h"

namespace nw {
namespace gfx {
namespace res {

struct ResParticleShapeData : public ResShapeData
{
    nw::ut::ResS32 m_VertexAttributesTableCount;
    nw::ut::Offset toVertexAttributesTable;
};

class ResParticleShape : public ResShape
{
public:
    enum
{ TYPE_INFO = NW_GFX_RES_TYPE_INFO(ResParticleShape) };
    enum
{ SIGNATURE = NW_RES_SIGNATURE32('PSHP') };

    NW_RES_CTOR_INHERIT(ResParticleShape, ResShape)

    s32 GetVertexAttributesCount() const
        { return ref().m_VertexAttributesTableCount; }
    ResVertexAttribute GetVertexAttributes(int idx)
        {
            nw::ut::Offset* tbl_ptr = static_cast<nw::ut::Offset*>(ref().toVertexAttributesTable.to_table_ptr());
            return ResVertexAttribute(tbl_ptr[idx].to_ptr());
        }
    const ResVertexAttribute GetVertexAttributes(int idx) const
        {
            const nw::ut::Offset* tbl_ptr = static_cast<const nw::ut::Offset*>(ref().toVertexAttributesTable.to_table_ptr());
            return ResVertexAttribute(tbl_ptr[idx].to_ptr());
        }
    nw::ut::internal::ResArray<ResVertexAttribute, nw::ut::internal::ResArrayClassTraits> GetVertexAttributes()
        {
            return nw::ut::internal::ResArray<ResVertexAttribute, nw::ut::internal::ResArrayClassTraits>(ref().toVertexAttributesTable.to_table_ptr(), GetVertexAttributesCount());
        }
    const nw::ut::internal::ResArray<const ResVertexAttribute, nw::ut::internal::ResArrayClassTraits> GetVertexAttributes() const
        {
            return nw::ut::internal::ResArray<const ResVertexAttribute, nw::ut::internal::ResArrayClassTraits>(ref().toVertexAttributesTable.to_table_ptr(), GetVertexAttributesCount());
        }

    void    Setup();
};

}
}
}

