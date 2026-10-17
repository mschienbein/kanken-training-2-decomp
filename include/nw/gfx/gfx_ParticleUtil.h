/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_ParticleUtil.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../types.h"
#include "../ut/ut_MoveArray.h"

#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <limits>
#include <functional>
#include <algorithm>

#include "res/gfx_ResParticleModel.h"
#include "res/gfx_ResParticleSet.h"

namespace nw {
namespace gfx {

class SceneNode;
class ParticleContext;

namespace res {
    class ResParticleInitializer;
    struct ResParticleInitializerData;
    class ResParticleUpdater;
    struct ResParticleUpdaterData;
} // namespace res

class ParticleUtil
{
public:
    static void SetupParticleObject(nw::ut::MoveArray<SceneNode*>* sceneNodeArray,ParticleContext* context);

    static void ResolveParticleObject(nw::ut::MoveArray<SceneNode*>* sceneNodeArray);

    static ResParticleInitializerData* DuplicateResParticleInitializer(const ResParticleInitializer* src,nw::os::IAllocator* allocator);

    static void GetMemorySizeForDuplicateResParticleInitializerInternal(nw::os::MemorySizeCalculator* pSize,const ResParticleInitializer* src);

    static ResParticleUpdaterData* DuplicateResParticleUpdater(const ResParticleUpdater* src,nw::os::IAllocator* allocator);

    static void GetMemorySizeForDuplicateResParticleUpdaterInternal(nw::os::MemorySizeCalculator* pSize,const ResParticleUpdater* src);
};

struct ParticleSetCompare : public std::binary_function<ParticleSet*, ParticleSet*, bool>
{
    bool operator() (const ParticleSet* lhs,const ParticleSet* rhs);
};

class ParticleSetIsBufferFlushEnabledSetter
{
public:
    ParticleSetIsBufferFlushEnabledSetter(bool flag): 
        m_Flag(flag) {}

    void operator()(ResModel resModel) const
    {
        ResParticleModel resParticleModel = ResDynamicCast<ResParticleModel>(resModel);
        if (resParticleModel.IsValid())
        {
            resParticleModel.ForeachParticleSet(ParticleSetIsBufferFlushEnabledSetter(m_Flag));
        }
    }

    void operator()(ResParticleModel resParticleModel) const
    {
        if (resParticleModel.IsValid())
        {
            resParticleModel.ForeachParticleSet(ParticleSetIsBufferFlushEnabledSetter(m_Flag));
        }
    }

    void operator()(ResParticleSet resParticleSet) const
    {
        resParticleSet.SetIsBufferFlushEnabled(m_Flag);
    }

private:
    bool m_Flag;
};

}
}

