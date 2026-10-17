/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_SceneUpdater.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_ISceneUpdater.h"
#include "gfx_RenderQueue.h"
#include "gfx_BillboardUpdater.h"
#include "gfx_WorldMatrixUpdater.h"
#include "gfx_SkeletonUpdater.h"
#include "gfx_SceneContext.h"

namespace nw{
namespace gfx{

class SkeletonUpdater;
class WorldMatrixUpdater;
class BillboardUpdater;

class SceneContext;

class SceneUpdater  : public ISceneUpdater
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(SceneUpdater);

public:
    NW_UT_RUNTIME_TYPEINFO;

    class Builder
    {
    public:
        Builder() {}

        Builder& SkeletonUpdaterPtr(SkeletonUpdater* skeletonUpdater)
        {
            this->m_SkeletonUpdater.Reset(skeletonUpdater);
            return *this;
        }

        Builder& WorldMatrixUpdaterPtr(WorldMatrixUpdater* worldMatrixUpdater)
        {
            this->m_WorldMatrixUpdater.Reset(worldMatrixUpdater);
            return *this;
        }

        Builder& BillboardUpdaterPtr(BillboardUpdater* billboardUpdater)
        {
            this->m_BillboardUpdater.Reset(billboardUpdater);
            return *this;
        }

        SceneUpdater* Create(os::IAllocator* allocator);

    private:
        GfxPtr<SkeletonUpdater> m_SkeletonUpdater;
        GfxPtr<WorldMatrixUpdater> m_WorldMatrixUpdater;
        GfxPtr<BillboardUpdater> m_BillboardUpdater;
    };

    virtual DepthSortMode GetDepthSortMode() const { return m_DepthSortMode; }

    virtual void SetDepthSortMode(DepthSortMode depthSortMode)
    {
        m_DepthSortMode = depthSortMode;
    }

    virtual void UpdateAll(SceneContext* sceneContext);

    void UpdateTransformNode(SceneContext* sceneContext) const;

    void UpdateSkeletalModel(SceneContext* sceneContext) const;

    void UpdateAnim(SceneContext* sceneContext) const;

    void EvaluateAnim(SceneContext* sceneContext, anim::ResGraphicsAnimGroup::EvaluationTiming timing) const;

    virtual void SubmitView(RenderQueue* renderQueue,SceneContext* sceneContext,const Camera& camera,
        u8 layerId,RenderSortMode renderSortMode = ALL_MESH_BASE_SORT);

    virtual void SubmitView(RenderQueue* renderQueue,SceneContext* sceneContext,const Camera& camera,
        u8 layerId,u8 particleLayerId,RenderSortMode renderSortMode = ALL_MESH_BASE_SORT);
    
    virtual void SubmitView(RenderQueue* renderQueue,SceneContext* sceneContext,const Camera& camera,
        u8 layerId,u8 particleLayerId,IsVisibleModelFunctor* isVisibleModel,RenderSortMode renderSortMode = ALL_MESH_BASE_SORT);
protected:
    SceneUpdater(nw::os::IAllocator* allocator,GfxPtr<SkeletonUpdater> skeletonUpdater,GfxPtr<WorldMatrixUpdater> worldMatrixUpdater,
    GfxPtr<BillboardUpdater> billboardUpdater): 
        ISceneUpdater(allocator),
        m_SkeletonUpdater(skeletonUpdater),
        m_WorldMatrixUpdater(worldMatrixUpdater),
        m_BillboardUpdater(billboardUpdater),
        m_DepthSortMode(SORT_DEPTH_OF_ALL_MESH) {}

    virtual ~SceneUpdater() {}

    GfxPtr<SkeletonUpdater> m_SkeletonUpdater;
    GfxPtr<WorldMatrixUpdater> m_WorldMatrixUpdater;
    GfxPtr<BillboardUpdater> m_BillboardUpdater;
    DepthSortMode m_DepthSortMode;
};

}
}

