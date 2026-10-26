/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr gfx_RenderContext.cpp, pinned commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * [INFERENCE] Native identity follows the mesh command-cache, uniform-bool and
 * vertex-attribute reset contracts; no historical source alias is asserted.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_ShaderProgram.h>
#include <nw/gfx/gfx_RenderContext.h>
#include <nw/gfx/gfx_ShaderUniforms.h>

void nw::gfx::RenderContext::ActivateVertexAttribute(ResMesh mesh)
{
    if (m_IsVertexAttributeDirty) {
        internal::ClearVertexAttribute();
        m_IsVertexAttributeDirty = false;
    }
    internal::NWUseCmdlist(mesh.ref().m_ActivateCommandCache,
                          mesh.ref().m_ActivateCommandCacheSize);
    internal::NWUseCmdlist(mesh.ref().m_IrScaleCommand,
                          sizeof(mesh.ref().m_IrScaleCommand));
    const bool hasVertexAlpha = (mesh.ref().m_Flags & ResMesh::FLAG_HAS_VERTEX_ALPHA) != 0;
    const bool hasBoneWeightW = (mesh.ref().m_Flags & ResMesh::FLAG_HAS_BONE_WEIGHT_W) != 0;
    m_ShaderProgram->SetVertexUniformBool(NW_GFX_VERTEX_UNIFORM(ISVERTA), hasVertexAlpha);
    m_ShaderProgram->SetVertexUniformBool(NW_GFX_VERTEX_UNIFORM(ISBONEW), hasBoneWeightW);
}
