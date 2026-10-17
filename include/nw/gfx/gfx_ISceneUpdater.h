/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_ISceneUpdater.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../ut/ut_MoveArray.h"
#include "gfx_RenderQueue.h"

namespace nw{
namespace gfx{

class SceneContext;
class SceneNode;
class Camera;

class ISceneUpdater : public GfxObject
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(ISceneUpdater);

public:
    NW_UT_RUNTIME_TYPEINFO;

    enum RenderSortMode
    {
        ALL_MESH_BASE_SORT,
        OPAQUE_MESH_BASE_AND_TRANSLUCENT_MODEL_BASE_SORT
    };

    enum DepthSortMode
    {
        SORT_DEPTH_OF_ALL_MESH,
        SORT_DEPTH_OF_TRANSLUCENT_MESH
    };

    class IsVisibleModelFunctor
    {
    public:
        virtual bool IsVisible(const Model* model)
        {
            NW_UNUSED_VARIABLE(model);
            return true;
        }
    };

    virtual DepthSortMode GetDepthSortMode() const = 0;

    virtual void SetDepthSortMode(DepthSortMode depthSortMode) = 0;

    virtual void UpdateAll(SceneContext* sceneContext) = 0;

    virtual void SubmitView(RenderQueue* renderQueue,SceneContext* sceneContext,const Camera& camera,u8 layerId,
        RenderSortMode renderSortMode = ALL_MESH_BASE_SORT) = 0;

    virtual void SubmitView(RenderQueue* renderQueue,SceneContext* sceneContext,const Camera& camera,u8 layerId,
        u8 particleLayerId,
        RenderSortMode renderSortMode = ALL_MESH_BASE_SORT) = 0;

    virtual void SubmitView(RenderQueue* renderQueue,SceneContext* sceneContext,const Camera& camera,u8 layerId,
        u8 particleLayerId,
        IsVisibleModelFunctor* isVisibleModel,
        RenderSortMode renderSortMode = ALL_MESH_BASE_SORT) = 0;


protected:

    ISceneUpdater(nw::os::IAllocator* allocator) : GfxObject(allocator) {}
};

}
}

