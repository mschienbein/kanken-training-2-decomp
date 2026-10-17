/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_ParticleSceneUpdater.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_ISceneUpdater.h"
#include "gfx_SkeletonUpdater.h"
#include "gfx_WorldMatrixUpdater.h"
#include "gfx_BillboardUpdater.h"

namespace nw{
namespace gfx{

class SceneContext;
class ParticleContext;
class ParticleSet;

class ParticleSceneUpdater  : public GfxObject
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(ParticleSceneUpdater);

public:
    NW_UT_RUNTIME_TYPEINFO;

    typedef nw::ut::MoveArray<SceneNode*> SceneNodeArray;

    typedef nw::ut::MoveArray<ParticleSet*> ParticleSetArray;

    class Builder
    {
    public:
        Builder() {}
        ParticleSceneUpdater* Create(nw::os::IAllocator* allocator);
    };

    void SetStepFrame(SceneContext* sceneContext, f32 stepFrame);
    virtual void UpdateNode(SceneContext* sceneContext, ParticleContext* particleContext, bool enableSwapBuffer = true);
    virtual void SwapBuffer(SceneContext*) {}

protected:
    ParticleSceneUpdater(nw::os::IAllocator* allocator): 
        GfxObject(allocator) {}

    virtual ~ParticleSceneUpdater() {}
};

}
}

