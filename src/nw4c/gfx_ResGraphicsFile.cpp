/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/res/gfx_ResGraphicsFile.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: gfx_ResGraphicsFile.cpp
//
// Project: NintendoWare4Ctr

#include <nw/ut/ut_ResUtil.h>
#include <nw/ut/ut_ResDictionary.h>
#include <nw/gfx/gfx_SceneObject.h>
#include <nw/gfx/res/gfx_ResShape.h>
#include <nw/gfx/res/gfx_ResModel.h>
#include <nw/gfx/res/gfx_ResMaterial.h>
#include <nw/gfx/res/gfx_ResGraphicsFile.h>
#include <nw/ut/ut_MiddlewareString.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2extern.h>

namespace nw {
namespace gfx {
namespace res {



void ResGraphicsFile::Cleanup()
{
    NW_ASSERT(this->IsValid());

    ut::SafeCleanupAll(this->GetModels());
    ut::SafeCleanupAll(this->GetLightAnims());
    ut::SafeCleanupAll(this->GetCameraAnims());
    ut::SafeCleanupAll(this->GetVisibilityAnims());
    ut::SafeCleanupAll(this->GetMaterialAnims());
    ut::SafeCleanupAll(this->GetSkeletalAnims());
    ut::SafeCleanupAll(this->GetTextures());
    ut::SafeCleanupAll(this->GetShaders());
    ut::SafeCleanupAll(this->GetFogs());
    ut::SafeCleanupAll(this->GetLights());
    ut::SafeCleanupAll(this->GetLutSets());
}

}
}
}

