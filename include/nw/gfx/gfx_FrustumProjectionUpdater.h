/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_FrustumProjectionUpdater.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_CameraProjectionUpdater.h"
#include "../ut/ut_Preprocessor.h"

namespace nw{
namespace os{
    class IAllocator;
}
namespace gfx{

class FrustumProjectionUpdater : public CameraProjectionUpdater
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(FrustumProjectionUpdater);

public:
    NW_UT_RUNTIME_TYPEINFO;

    static FrustumProjectionUpdater* Create(nw::os::IAllocator* allocator);
    static FrustumProjectionUpdater* Create(nw::os::IAllocator* allocator,ResFrustumProjectionUpdater resUpdater);

    static void GetMemorySizeInternal(nw::os::MemorySizeCalculator* pSize,bool isDynamicBuild)
    {
        nw::os::MemorySizeCalculator& size = *pSize;

        size += sizeof(FrustumProjectionUpdater);
        if (isDynamicBuild)
        {
            size += sizeof(ResFrustumProjectionUpdaterData);
        }
    }

    void virtual Update(nw::math::MTX44* projectionMatrix, nw::math::MTX34* textureProjectionMatrix);

    virtual ResCameraProjectionUpdater GetResource()
    {
        return this->m_Resource;
    }

    virtual const ResCameraProjectionUpdater GetResource() const
    {
        return this->m_Resource;
    }

    virtual anim::ResCameraAnimData::ProjectionUpdaterKind Kind() const
    {
        return anim::ResCameraAnimData::PROJECTION_UPDATER_FRUSTUM;
    }
    
private:
    FrustumProjectionUpdater(nw::os::IAllocator* pAllocator,bool isDynamic,ResFrustumProjectionUpdater resUpdater);

    virtual ~FrustumProjectionUpdater();

    ResFrustumProjectionUpdater m_Resource;
};

}
}

