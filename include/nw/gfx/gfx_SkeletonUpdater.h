/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_SkeletonUpdater.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_GfxObject.h"
#include "gfx_Skeleton.h"
#include "gfx_CalculatedTransform.h"

namespace nw{
namespace os{
    class IAllocator;
}
namespace gfx{
class Skeleton;
class WorldMatrixUpdater;
class BillboardUpdater;
class Camera;

class SkeletonUpdater : public GfxObject
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(SkeletonUpdater);

public:
    class Builder
    {
    public:
        Builder() {}

        SkeletonUpdater* Create(nw::os::IAllocator* allocator);
    };

    void UpdateWorld(Skeleton* skeleton,const WorldMatrixUpdater& worldMatrixUpdater) const;

    void UpdateView(Skeleton* skeleton,const BillboardUpdater& billboardUpdater,const Camera& camera) const;

private:
    SkeletonUpdater(nw::os::IAllocator* allocator);
    virtual ~SkeletonUpdater();
};
}
}

