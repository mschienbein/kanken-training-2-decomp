/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/res/gfx_ResFragmentShader.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: gfx_ResFragmentShader.cpp
//
// Project: NintendoWare4Ctr

#include <nw/gfx/res/gfx_ResUtil.h>
#include <nw/gfx/res/gfx_ResFragmentShader.h>
#include <nw/gfx/res/gfx_ResGraphicsFile.h>

namespace nw {
namespace gfx {
namespace res {







void ResFragmentShader::Cleanup()
{
    ut::SafeCleanup(this->GetFragmentLightingTable());
}

void ResFragmentLightingTable::Cleanup()
{
    ut::SafeCleanup(this->GetReflectanceRSampler());
    ut::SafeCleanup(this->GetReflectanceGSampler());
    ut::SafeCleanup(this->GetReflectanceBSampler());
    ut::SafeCleanup(this->GetDistribution0Sampler());
    ut::SafeCleanup(this->GetDistribution1Sampler());
    ut::SafeCleanup(this->GetFresnelSampler());
}

}
}
}

