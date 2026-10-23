/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr, commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd: gfx_MaterialState.cpp and
 * math_Matrix44.ipp. NintendoWare4Ctr contributors.
 * Modified 2026-10-17: canonical matrix type, address-named entry and
 * independently expressed identity holder for the missing SDK constructor body.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResMaterial.h>
#include <nw/math/inlines/math_Matrix44.ipp>

/* [INFERENCE] SDK matrix default construction leaves its scalar storage
 * uninitialized; the canonical header currently declares but does not define it.
 */
inline nn::math::MTX44::MTX44() {}
struct R41270IdentityMatrix {
    nw::math::MTX44 value;
    R41270IdentityMatrix()
    {
        value.m[0][0] = 1.0f; value.m[0][1] = 0.0f;
        value.m[0][2] = 0.0f; value.m[0][3] = 0.0f;
        value.m[1][0] = 0.0f; value.m[1][1] = 1.0f;
        value.m[1][2] = 0.0f; value.m[1][3] = 0.0f;
        value.m[2][0] = 0.0f; value.m[2][1] = 0.0f;
        value.m[2][2] = 1.0f; value.m[2][3] = 0.0f;
        value.m[3][0] = 0.0f; value.m[3][1] = 0.0f;
        value.m[3][2] = 0.0f; value.m[3][3] = 1.0f;
    }
};
static inline nw::math::MTX44* texture_identity(nw::math::MTX44* output)
{
    static const R41270IdentityMatrix identity;
    return nn::math::MTX44Copy(output, &identity.value);
}
extern "C" nw::math::MTX44* func_001c9e08(nw::math::MTX44* matrix,
    nw::gfx::res::ResTextureCoordinator::MappingMatrixMode mode,
    float scale_s, float scale_t, float rotate, float translate_s, float translate_t)
{
    switch (mode)
    {
    case nw::gfx::res::ResTextureCoordinator::MAPPINGMATRIXMODE_MAYA:
        nw::math::MTX44TextureMatrixForMaya(matrix, scale_s, scale_t, rotate, translate_s, translate_t);
        break;
    case nw::gfx::res::ResTextureCoordinator::MAPPINGMATRIXMODE_SOFTIMAGE:
        nw::math::MTX44TextureMatrixForSoftimage(matrix, scale_s, scale_t, rotate, translate_s, translate_t);
        break;
    case nw::gfx::res::ResTextureCoordinator::MAPPINGMATRIXMODE_3DSMAX:
        nw::math::MTX44TextureMatrixForMax(matrix, scale_s, scale_t, rotate, translate_s, translate_t);
        break;
    default:
        texture_identity(matrix);
        break;
    }
    return matrix;
}
