#ifndef GAME_GRAPHICS_RESOURCE_ABI_H
#define GAME_GRAPHICS_RESOURCE_ABI_H

#ifdef __cplusplus
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResTexture.h>
#include <nw/gfx/res/gfx_ResTextureMapper.h>
#include <nw/gfx/res/gfx_ResVertex.h>
#include <nw/gfx/res/gfx_ResLookupTable.h>
#include <nw/os/os_Memory.h>
#include "game/resource_dispatch.h"
#include "game/render_state.h"

namespace nw { namespace gfx { namespace res {
/* [INFERENCE] Target-version serialized prefix: both original cube consumers
 * use six slot-relative offsets at +0x38. This is not the complete resource
 * extent and never substitutes for the public toImagesTables array at +0x50. */
struct TargetCubeTextureB2KJData : ResPixelBasedTextureData {
    nw::ut::Offset face_images[ResCubeTexture::MAX_CUBE_FACE];
};
class TargetCubeTextureB2KJ : public ResPixelBasedTexture {
public:
    enum { TYPE_INFO = ResCubeTexture::TYPE_INFO };
    enum { MAX_CUBE_FACE = ResCubeTexture::MAX_CUBE_FACE };
    typedef ResCubeTexture::CubeFace CubeFace;
    NW_RES_CTOR_INHERIT(TargetCubeTextureB2KJ, ResPixelBasedTexture)
    ResPixelBasedImage GetImage(CubeFace face)
    {
        return ResPixelBasedImage(ref().face_images[face].to_ptr());
    }
};
inline ResPixelBasedImage target_cube_image(ResCubeTexture texture,
                                           ResCubeTexture::CubeFace face)
{
    TargetCubeTextureB2KJ target(texture.ptr());
    return target.GetImage(face);
}

extern "C" {
void func_001c73cc(u32 buffer_id, ResVertexStreamBase stream, u32 load_flags);
void func_001c7a24(ResVertexAttribute *attribute);
void func_0016bbdc(u32 texture_id, ResPixelBasedImage image, u32 load_flags);
void func_001c7d00(ResTexture texture);
void func_001c7e1c(ResTexture texture);
void func_001c7b30(ResTexture texture);
void func_001c80cc(ResTexture texture);
}
void SetupTextureMapperCommand(ResPixelBasedTextureMapper mapper, ResCubeTexture texture);
} } }

extern "C" {
nw::gfx::res::ResTexture func_001c7828(nw::os::IAllocator *allocator,
                                      nw::gfx::res::ResTexture texture);
nw::gfx::res::ResTextureMapper func_001c8c3c(nw::gfx::res::ResTextureMapper mapper,
                                            nw::os::IAllocator *allocator);
}
#endif

#endif
