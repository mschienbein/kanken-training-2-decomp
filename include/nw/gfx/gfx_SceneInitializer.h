/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_SceneInitializer.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_ISceneVisitor.h"
#include "gfx_IMaterialIdGenerator.h"

namespace nw{
namespace gfx{


class SceneInitializer : public ISceneVisitor
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(SceneInitializer);

public:
    NW_UT_RUNTIME_TYPEINFO;

    struct Description
    {
        IMaterialIdGenerator* materialIdGenerator;

        Description(): 
            materialIdGenerator(NULL) {}
    };

    class Builder
    {
    public:
        Builder& MaterialIdGenerator(IMaterialIdGenerator* materialIdGenerator)
        {
            m_Description.materialIdGenerator = materialIdGenerator;
            return *this;
        }

        SceneInitializer* Create(nw::os::IAllocator* allocator);

    private:
        Description m_Description;
    };

    void Begin();
    void End();
    
    virtual void VisitSceneNode(SceneNode*) {}
    virtual void VisitTransformNode(TransformNode*) {}
    virtual void VisitUserRenderNode(UserRenderNode*) {}
    virtual void VisitModel(Model* model);
    virtual void VisitSkeletalModel(SkeletalModel* model);
    virtual void VisitCamera(Camera*) {}
    virtual void VisitFog(Fog*) {}
    virtual void VisitLight(Light*) {}
    virtual void VisitFragmentLight(FragmentLight*) {}
    virtual void VisitVertexLight(VertexLight*) {}
    virtual void VisitAmbientLight(AmbientLight*) {}
    virtual void VisitHemiSphereLight(HemiSphereLight*) {}
    virtual void VisitParticleSet(ParticleSet*) {}
    virtual void VisitParticleEmitter(ParticleEmitter*) {}
    virtual void VisitParticleModel(ParticleModel*);
    
private:

    SceneInitializer(nw::os::IAllocator* allocator, Description description): 
        ISceneVisitor(allocator),
        m_MaterialIdGenerator(description.materialIdGenerator) {}

    virtual ~SceneInitializer() {}

    GfxPtr<IMaterialIdGenerator> m_MaterialIdGenerator;
};

}
}

