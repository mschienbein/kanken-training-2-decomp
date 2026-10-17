/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_BillboardUpdater.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_GfxObject.h"
#include "res/gfx_ResSkeleton.h"

namespace nw{
namespace os{
    class IAllocator;
}
namespace gfx{

class Matrix34;

class BillboardUpdater : public GfxObject
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(BillboardUpdater);

public:

    static BillboardUpdater* Create(nw::os::IAllocator* allocator);


    void Update(nw::math::MTX34* worldMatrix,const nw::math::MTX34& viewMatrix,const nw::math::MTX34& inverseViewMatrix,const nw::math::VEC3& cameraPosition,
        const CalculatedTransform& worldTransform,
        const CalculatedTransform& localTransform,
        ResBone::BillboardMode billboardMode) const;

private:
    BillboardUpdater(nw::os::IAllocator* allocator);

    virtual ~BillboardUpdater();

    void CalculateLocalMatrix(nw::math::MTX34* localMatrix,const CalculatedTransform& transform,nw::math::VEC3 zAxis,bool recalculateYAxis = true) const;

    void CalculateScreenLocalMatrix(nw::math::MTX34* localMatrix,const CalculatedTransform& transform,
        const nw::math::MTX34& inverseViewMatrix,
        nw::math::VEC3 yAxis, nw::math::VEC3& zAxis) const;
};

}
}

