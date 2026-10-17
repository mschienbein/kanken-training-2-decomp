/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_GlImplement.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../types.h"

#include "../../gles2/gl2.h"
#include "../../gles2/gl2extern.h"
#include "../../nn/gx.h"
#include <string.h>
#include "../../nn/gx/CTR/gx_CommandAccess.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PER_PIXEL_LIGHTS            8

typedef void* (*dmpgl_allocator_t)(GLenum, GLenum, GLuint, GLsizei);
typedef void (*dmpgl_deallocator_t)(GLenum, GLenum ,GLuint, void*);

extern dmpgl_allocator_t __dmpgl_allocator;
extern dmpgl_deallocator_t  __dmpgl_deallocator;

extern unsigned* __cb_current_command_buffer;
extern unsigned* __cb_current_max_command_buffer;

#ifdef __cplusplus
}
#endif

namespace nw { /* nw */
namespace gfx {
namespace internal {

void nwgfxAddVramDmaCommand(void* srcaddr, void* dstaddr, GLsizei size);
void nwgfxClear(u32 colorAddr, u32 colorSize, u32 clearColor, u32 colorWidth,u32 depthAddr, u32 depthSize, u32 clearDepth, u32 depthWidth);

void GetFrameBufferState(GLuint fboID, u32* pColorAddr, u32* pDepthAddr);

}
}
}

