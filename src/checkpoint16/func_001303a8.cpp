/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr gfx_MeshRenderer.cpp at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd; see src/nw4c/LICENSE.txt.
 * [INFERENCE] Native method identity; original TU membership is not established.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_GfxObject.h>
#include <nw/gfx/gfx_MeshRenderer.h>
#include <nw/gfx/gfx_RenderContext.h>
#include <nw/gfx/gfx_ShaderProgram.h>
#include <nw/gfx/gfx_SkeletalModel.h>
#include <nw/gfx/gfx_Skeleton.h>
#include <nw/gfx/gfx_ActivateCommand.h>

namespace nw { namespace gfx {
void MeshRenderer::SetMatrixPalette(SkeletalModel* skeletalModel,
    ResPrimitiveSet primitiveSet, s32 boneIndexCount)
{
    const int UNIT_COUNT = 3;
    const ShaderProgram* shaderProgram = m_RenderContext->GetShaderProgram();
    bool isRigid = primitiveSet.GetSkinningMode() != ResPrimitiveSet::SKINNING_MODE_SMOOTH;
    shaderProgram->SetVertexUniformBool(NW_GFX_VERTEX_UNIFORM(ISSMOSK), !isRigid);
    shaderProgram->SetVertexUniformBool(NW_GFX_VERTEX_UNIFORM(ISRGDSK), isRigid);
    Skeleton* skeleton = skeletalModel->GetSkeleton();
    ResSkeleton resSkeleton = skeleton->GetResSkeleton();
    Skeleton::MatrixPose& matrixPose = skeleton->WorldMatrixPose();
    Skeleton::MatrixPose& skiningPose = skeleton->SkiningMatrixPose();
    for (int count = 0; count < boneIndexCount; ++count) {
        s32 boneIndex = primitiveSet.GetBoneIndexTable(count);
        ut::ResDicPatricia bones(resSkeleton.ref().toBonesDic.to_ptr());
        ResBone bone(bones[boneIndex]);
        bool hasSkinningMatrix = (bone.GetFlags() & ResBoneData::FLAG_HAS_SKINNING_MATRIX) != 0;
        Skeleton::MatrixPose* pose = (!isRigid && hasSkinningMatrix) ? &skiningPose : &matrixPose;
        math::MTX34* matrix = pose->GetMatrix(boneIndex);
        int index = count * UNIT_COUNT;
        if (count == 0)
            internal::NWSetVertexUniform4fvBegin(VERTEX_SHADER_UNIFORM_UNIVREG_INDEX + index,
                boneIndexCount * UNIT_COUNT, UNIT_COUNT, *matrix);
        else
            internal::NWSetVertexUniform4fvContinuous(UNIT_COUNT, *matrix);
    }
    internal::NWSetVertexUniform4fvEnd();
}
} }
