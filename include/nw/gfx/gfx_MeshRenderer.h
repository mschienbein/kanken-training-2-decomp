/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_MeshRenderer.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../ut/ut_ResUtil.h"
#include "../ut/ut_ResDictionary.h"
#include "res/gfx_ResModel.h"
#include "res/gfx_ResShape.h"
#include "res/gfx_ResParticleShape.h"
#include "res/gfx_ResMaterial.h"
#include "res/gfx_ResMesh.h"

namespace nw{
namespace os{
    class IAllocator;
}
namespace gfx{

class Model;
class SkeletalModel;
class Mesh;
class RenderContext;
class PrimitiveSet;
class Primitive;

class MeshRenderer : public GfxObject
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(MeshRenderer);

public:
    static MeshRenderer* Create(nw::os::IAllocator* pAllocator);

    void SetRenderContext(RenderContext* renderContext)
    {
        m_RenderContext = renderContext;
    }
    
    void RenderMesh(ResMesh mesh, Model* model);

private:    
    MeshRenderer(nw::os::IAllocator* allocator): 
        GfxObject(allocator),
        m_RenderContext(NULL) {}

    virtual ~MeshRenderer() {}

    void RenderSeparateDataShape(Model* model,ResSeparateDataShape shape,s32 currentPrimitiveIndex);

    void RenderParticleShape(Model* model,ResParticleShape shape,int index);

    void SetMatrixPalette(SkeletalModel* skeletalModel,ResPrimitiveSet primitiveSet,s32 boneIndexCount);

    RenderContext*      m_RenderContext;
};

}
}

