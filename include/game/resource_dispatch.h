#ifndef GAME_RESOURCE_DISPATCH_H
#define GAME_RESOURCE_DISPATCH_H

#ifdef __cplusplus
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResLookupTable.h>
#include <nw/gfx/res/gfx_ResTextureMapper.h>

/* These are native one-pointer resource values, not pointers to the resource
 * wrapper. Each original dispatcher supplies its serialized payload in r0;
 * the tag selects the provider's projection within the same resource family.
 * The data objects remain external and receive no source/data credit. */
extern "C" {
extern void (*data_002434e4[2])(nw::gfx::res::ResLookupTable resource);
extern void (*data_002437e0[2])(nw::gfx::res::ResTextureMapper resource);
extern void (*data_00243510[5])(nw::gfx::res::ResTexture resource);

void func_001c6a34(nw::gfx::res::ResLookupTable resource);
void func_001c7f10(nw::gfx::res::ResLookupTable resource);
void func_001c8af0(nw::gfx::res::ResTextureMapper resource);
void func_001c8b1c(nw::gfx::res::ResTextureMapper resource);
void func_001c7e90(nw::gfx::res::ResTexture resource);
void func_001c7d7c(nw::gfx::res::ResTexture resource);
void func_001c8984(nw::gfx::res::ResTexture resource);
void func_001c8990(nw::gfx::res::ResTexture resource);
void func_001c7f20(nw::gfx::res::ResTexture resource);
}
#endif

#endif
