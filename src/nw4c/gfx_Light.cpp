/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/gfx_Light.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: gfx_Light.cpp
//
// Project: NintendoWare4Ctr

#include <nw/gfx/gfx_Light.h>
#include <nw/gfx/gfx_HemiSphereLight.h>
#include <nw/gfx/gfx_VertexLight.h>
#include <nw/gfx/gfx_ISceneVisitor.h>
#include <nw/gfx/gfx_AmbientLight.h>

namespace nw{
namespace gfx{

void Light::DestroyOriginalValue()
{
    NW_ASSERT(m_OriginalValue.IsValid());

    GetAllocator().Free(this->m_OriginalValue.ptr());
    m_OriginalValue = ResLight(NULL);
}





}
}

