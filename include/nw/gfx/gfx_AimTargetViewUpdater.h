/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_AimTargetViewUpdater.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_CameraViewUpdater.h"
#include "../ut/ut_Preprocessor.h"

namespace nw{
namespace os{
    class IAllocator;
}

namespace gfx{

class AimTargetViewUpdater : public CameraViewUpdater
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(AimTargetViewUpdater);

public:
    NW_UT_RUNTIME_TYPEINFO;

    static AimTargetViewUpdater* Create(nw::os::IAllocator* allocator);

    static AimTargetViewUpdater* Create(nw::os::IAllocator* allocator, ResAimTargetViewUpdater resUpdater);

    static void GetMemorySizeInternal(nw::os::MemorySizeCalculator* pSize, bool isDynamicBuild)
    {
        nw::os::MemorySizeCalculator& size = *pSize;

        size += sizeof(AimTargetViewUpdater);
        if (isDynamicBuild)
        {
            size += sizeof(ResAimTargetViewUpdaterData);
        }
    }

    void virtual Update(nw::math::MTX34* viewMatrix, const nw::math::MTX34& worldMatrix, const nw::math::VEC3& cameraPosition);

    virtual ResCameraViewUpdater GetResource() { return m_Resource; }

    virtual const ResCameraViewUpdater GetResource() const { return m_Resource; }

    virtual anim::ResCameraAnimData::ViewUpdaterKind Kind() const
    {
        return anim::ResCameraAnimData::VIEW_UPDATER_AIM;
    }
    
private:
    AimTargetViewUpdater(nw::os::IAllocator* pAllocator, bool isDynamic, ResAimTargetViewUpdater resUpdater);
    virtual ~AimTargetViewUpdater();

    ResAimTargetViewUpdater m_Resource;
};

}
}

