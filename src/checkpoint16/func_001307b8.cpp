/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr gfx_RenderContext.cpp, pinned commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * [INFERENCE] Native model-matrix method identity follows the two uniform
 * matrix uploads and the model cache write. The established shared game
 * identity provider is reused without asserting a second static-data alias.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_ShaderProgram.h>
#include <nw/gfx/gfx_RenderContext.h>
#include "game/matrix.h"

void nw::gfx::RenderContext::SetModelMatrixForModel(Model *model)
{
    const Matrix34 *identity = matrix34Identity();
    internal::NWSetVertexUniform4fv(VERTEX_SHADER_UNIFORM_WRLDMTX_INDEX, 3,
                                  identity->elements);
    m_ShaderProgram->SetModelNormalMatrix(model->NormalMatrix());
    m_ModelCache = model;
}
