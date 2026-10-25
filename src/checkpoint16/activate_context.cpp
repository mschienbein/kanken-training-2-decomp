/* SPDX-License-Identifier: GPL-3.0-only
 * Native renderer algorithm follows NintendoWare4Ctr gfx_RenderContext.cpp at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd; see src/nw4c/LICENSE.txt.
 * [INFERENCE] Native NW4C identity follows the canonical class, owner/material
 * layout, virtual activator slot and direct-call contracts. */
#define NW_RELEASE
#include <preinclude.h>
#include <nw/gfx/gfx_RenderContext.h>
#include <nw/gfx/gfx_IMaterialActivator.h>

#include "game/render_context.h"

namespace nw { namespace gfx {
void RenderContext::ActivateContext(IMaterialActivator *userMaterialActivator)
{
    Material *material = m_Material;
    if (m_MaterialCache == material)
        return;
    ActivateShaderProgram();
    func_001384a0(this);
    ActivateMaterial(userMaterialActivator);
    m_SceneEnvironment.SetAllFlagsDirty(false);
    m_MaterialCache = m_Material;
}
inline void RenderContext::ActivateShaderProgram()
{
    if (!ut::CheckFlag(m_RenderMode, RENDERMODE_IGNORE_SHADER)) {
        ResShaderProgramDescription resource = m_Material->GetDescription();
        ResShaderProgramDescription previous = m_ShaderProgram->GetActiveDescription();
        if (previous != resource) {
            if (previous.IsValid() && previous.GetGeometryShaderIndex() >= 0 &&
                resource.GetGeometryShaderIndex() < 0) {
                m_IsVertexAttributeDirty = true;
                func_00191c94(&m_MaterialHash, 0x1015);
                m_SceneEnvironment.SetAllFlagsDirty(true);
                m_ModelCache = 0;
            }
            func_0013862c(m_ShaderProgram.Get(), resource);
            m_IsShaderProgramDirty = true;
        } else {
            m_IsShaderProgramDirty = false;
        }
    }
}
} }
