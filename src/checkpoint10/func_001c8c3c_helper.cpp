/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr gfx_ResTextureMapper.cpp,
 * ctrdecomp/NintendoWare4Ctr commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified: address-named entry point and observed texture-reference call.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResTextureMapper.h>
using namespace nw::gfx;
/* INTEGRATOR: move to include/game/texture_mapper.h */
extern "C" {
ResTexture func_001c7828(nw::os::IAllocator *allocator, ResTexture texture);
}
static void destroy_dynamic(ResTextureMapper mapper, nw::os::IAllocator *allocator)
{
    ResProceduralTextureMapper procedural = nw::ut::ResStaticCast<ResProceduralTextureMapper>(mapper);
    if (procedural.GetTexture().IsValid())
        allocator->Free(procedural.ref().toTexture.to_ptr());
    allocator->Free(procedural.ptr());
}
extern "C" ResTextureMapper func_001c8c3c(ResTextureMapper mapper, nw::os::IAllocator *allocator)
{
    ResProceduralTextureMapper procedural = nw::ut::ResStaticCast<ResProceduralTextureMapper>(mapper);
    void *memory = allocator->Alloc(sizeof(ResProceduralTextureMapperData));
    if (!memory)
        return ResTextureMapper(NULL);
    ResProceduralTextureMapperData *copy = new(memory) ResProceduralTextureMapperData(procedural.ref());
    ResProceduralTextureMapper clone(copy);
    ResTexture texture = procedural.GetTexture();
    if (texture.IsValid()) {
        ResTexture reference = func_001c7828(allocator, texture);
        if (reference.IsValid())
            copy->toTexture.set_ptr(reference.ptr());
        else {
            /* ORIGINAL BUG: failure cleanup can free copied, unrebased texture offsets. */
            destroy_dynamic(clone, allocator);
            return ResTextureMapper(NULL);
        }
    }
    return clone;
}
