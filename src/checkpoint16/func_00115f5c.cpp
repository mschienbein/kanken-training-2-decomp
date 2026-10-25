/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr gfx_MeshRenderer.cpp, pinned commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * [INFERENCE] Native RenderMesh identity follows the shape dispatch, material
 * selection, command-cache submission and particle-count guard contracts.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_GfxObject.h>
#include <nw/gfx/gfx_MeshRenderer.h>
#include <nw/gfx/gfx_RenderContext.h>
#include <nw/gfx/gfx_ParticleModel.h>
#include <nw/gfx/gfx_ParticleSet.h>
#include <nw/gfx/gfx_ParticleCollection.h>

static nw::gfx::Material *material_for_mesh(nw::gfx::Model *model, nw::gfx::ResMesh mesh)
{
    return model->GetMaterial(mesh.GetMaterialIndex());
}

void nw::gfx::MeshRenderer::RenderMesh(ResMesh mesh, Model *model)
{
    ResShape shape = model->GetResModel().GetShapes(mesh.GetShapeIndex());
    switch (shape.ref().typeInfo) {
    case ResSeparateDataShape::TYPE_INFO:
        m_RenderContext->SetMaterial(material_for_mesh(model, mesh));
        m_RenderContext->ActivateContext();
        m_RenderContext->ActivateVertexAttribute(mesh);
        RenderSeparateDataShape(model, ResStaticCast<ResSeparateDataShape>(shape),
                                mesh.GetCurrentPrimitiveIndex());
        m_RenderContext->DeactivateVertexAttribute(mesh);
        break;
    case ResParticleShape::TYPE_INFO: {
        ParticleModel *particleModel = static_cast<ParticleModel *>(model);
        ParticleSet *particleSet = particleModel->GetParticleSets(mesh.GetShapeIndex());
        if (particleSet->GetParticleCollection()->GetCount() == 0)
            return;
        m_RenderContext->SetMaterial(material_for_mesh(model, mesh));
        m_RenderContext->ActivateParticleContext();
        RenderParticleShape(model, ResStaticCast<ResParticleShape>(shape), mesh.GetShapeIndex());
        break;
    }
    }
}
