/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/gfx_FragmentLight.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: gfx_FragmentLight.cpp
//
// Project: NintendoWare4Ctr

#include <nw/os/os_Memory.h>

#include <nw/gfx/gfx_FragmentLight.h>
#include <nw/gfx/gfx_ISceneVisitor.h>

#include <cstring>

namespace nw{
namespace gfx{

void FragmentLight::DestroyResFragmentLight(nw::os::IAllocator* allocator, ResFragmentLightData* resFragmentLight)
{
    NW_NULL_ASSERT(allocator);
    NW_NULL_ASSERT(resFragmentLight);

    allocator->Free(resFragmentLight->toDistanceSampler.to_ptr());

    ResLightingLookupTableData* angleSampler = reinterpret_cast<ResLightingLookupTableData*>(resFragmentLight->toAngleSampler.to_ptr());
    if (angleSampler->toSampler.to_ptr() != NULL)
    {
        allocator->Free(angleSampler->toSampler.to_ptr());
    }
    allocator->Free(angleSampler);

    if (resFragmentLight->toName.to_ptr() != NULL)
    {
        allocator->Free(const_cast<char*>(resFragmentLight->toName.to_ptr()));
    }

    allocator->Free(resFragmentLight);
}







}
}

