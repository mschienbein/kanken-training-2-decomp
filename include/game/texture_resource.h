#ifndef GAME_TEXTURE_RESOURCE_H
#define GAME_TEXTURE_RESOURCE_H
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResTexture.h>
#include <nw/gfx/res/gfx_ResGraphicsFile.h>
#include <nw/gfx/res/gfx_ResUtil.h>

/* [INFERENCE] Setup-helper identity. The actual caller keeps the canonical
 * pointer/reference resource views and the native Result value contract. */
extern "C" nw::gfx::Result func_001c74e8(nw::gfx::res::ResTexture *texture,
    nw::os::IAllocator *allocator, nw::gfx::res::ResGraphicsFile file);
/* [INFERENCE] By-value setup hook roles follow the native resource callers. */
extern "C" void (*data_002434fc[5])(nw::gfx::res::ResTexture texture);
#endif
