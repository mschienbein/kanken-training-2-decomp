/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr gfx_ResTextureMapper.cpp,
 * ctrdecomp/NintendoWare4Ctr commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified: address-named entry point; canonical native declarations retained.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResTextureMapper.h>

using namespace nw::gfx;

extern "C" void func_001c8d14(ResTextureMapper mapper, nw::os::IAllocator *allocator)
{
    ResPixelBasedTextureMapper sampled = nw::ut::ResStaticCast<ResPixelBasedTextureMapper>(mapper);
    if (sampled.GetSampler().IsValid())
        allocator->Free(sampled.ref().toSampler.to_ptr());
    if (sampled.GetTexture().IsValid())
        allocator->Free(sampled.ref().toTexture.to_ptr());
    allocator->Free(sampled.ptr());
}
