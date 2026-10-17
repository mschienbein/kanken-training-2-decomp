/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_PerspectiveProjectionUpdater.h
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

class PerspectiveProjectionUpdater : public CameraProjectionUpdater
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(PerspectiveProjectionUpdater);

public:
    NW_UT_RUNTIME_TYPEINFO;

    static PerspectiveProjectionUpdater* Create(nw::os::IAllocator* allocator);
    static PerspectiveProjectionUpdater* Create(nw::os::IAllocator* allocator,ResPerspectiveProjectionUpdater resUpdater);

    static void GetMemorySizeInternal(nw::os::MemorySizeCalculator* pSize,bool isDynamicBuild)
    {
        nw::os::MemorySizeCalculator& size = *pSize;

        size += sizeof(PerspectiveProjectionUpdater);
        if (isDynamicBuild)
        {
            size += sizeof(ResPerspectiveProjectionUpdaterData);
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
        return anim::ResCameraAnimData::PROJECTION_UPDATER_PERSPECTIVE;
    }
    
private:

    PerspectiveProjectionUpdater(nw::os::IAllocator* pAllocator,bool isDynamic,ResPerspectiveProjectionUpdater resUpdater);

    virtual ~PerspectiveProjectionUpdater();

    ResPerspectiveProjectionUpdater m_Resource;
};

}
}

