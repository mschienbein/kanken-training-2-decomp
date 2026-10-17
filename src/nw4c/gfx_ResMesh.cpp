/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/res/gfx_ResMesh.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: gfx_ResMesh.cpp
//
// Project: NintendoWare4Ctr

#include <nw/gfx/res/gfx_ResMesh.h>
#include <nw/gfx/res/gfx_ResModel.h>
#include <nw/gfx/res/gfx_ResGraphicsFile.h>
#include <nw/gfx/gfx_ActivateCommand.h>
#include <nw/gfx/gfx_ShaderUniforms.h>
#include <nw/ut/ut_Float24.h>
#include <nn/gx.h>

namespace nw {
namespace gfx {
namespace res {

namespace {

    enum
    {
        BONE_INDEX_SCALE_INDEX = -2
    };

}

void ResMesh::Cleanup()
{
    this->DisableFlags(ResMesh::FLAG_HAS_BEEN_SETUP);

    if (ref().m_ActivateCommandCache != NULL)
    {
        ref().m_CommandAllocator->Free(ref().m_ActivateCommandCache);

        ref().m_ActivateCommandCache = NULL;
        ref().m_ActivateCommandCacheSize = 0;

        if (ref().m_DeactivateCommandCache != NULL)
        {
            ref().m_DeactivateCommandCache = NULL;
            ref().m_DeactivateCommandCacheSize = 0;
        }
    }

    NW_ASSERT(ref().m_DeactivateCommandCache == NULL);
}

} // namespace res
} // namespace gfx
} // namespace nw

