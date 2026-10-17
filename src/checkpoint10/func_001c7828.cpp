/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/gfx/res/gfx_ResTextureMapper.cpp, ReferResTexture.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResTexture.h>
#include <nw/os/os_Memory.h>
#include <new>

extern "C" nw::gfx::res::ResTexture func_001c7828(nw::os::IAllocator *allocator,
    nw::gfx::res::ResTexture source)
{
    using namespace nw::gfx;
    void *memory = allocator->Alloc(sizeof(ResReferenceTextureData));
    if (memory == NULL)
        return ResTexture(NULL);
    ResReferenceTextureData *texture = new(memory) ResReferenceTextureData();
    texture->typeInfo = ResReferenceTexture::TYPE_INFO;
    switch (source.ref().typeInfo) {
    case ResImageTexture::TYPE_INFO:
        texture->toTargetTexture.set_ptr(source.ptr());
        break;
    case ResCubeTexture::TYPE_INFO:
        texture->toTargetTexture.set_ptr(source.ptr());
        break;
    case ResReferenceTexture::TYPE_INFO: {
        ResReferenceTexture reference = ResDynamicCast<ResReferenceTexture>(source);
        texture->toTargetTexture.set_ptr(reference.GetTargetTexture().ptr());
        break;
    }
    case ResProceduralTexture_TYPE_INFO:
        texture->toTargetTexture.set_ptr(source.ptr());
        break;
    }
    return ResTexture(texture);
}
