/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; sources/libraries/gfx/gfx_FragmentLight.cpp
 * Pinned upstream: 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified 2026-10-20: select real native storage provider for identity diagnosis.
 * GNU GPL version 3; see src/nw4c/LICENSE.txt.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_FragmentLight.h>
namespace nw { namespace gfx {
Result FragmentLight::CreateOriginalValue(nw::os::IAllocator* allocator)
{
    Result result = INITIALIZE_RESULT_OK;
    void* buffer = allocator->Alloc(sizeof(ResFragmentLightData));
    NW_NULL_ASSERT(buffer);
    /* ORIGINAL BUG: allocation failure is not checked before the native copy. */
    ResFragmentLightData* original = new(buffer) ResFragmentLightData(GetResFragmentLight().ref());
    m_OriginalValue = ResFragmentLight(original);
    m_OriginalTransform = GetResTransformNode().GetTransform();
    return result;
}
} }
