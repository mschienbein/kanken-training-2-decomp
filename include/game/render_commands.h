#ifndef GAME_RENDER_COMMANDS_H
#define GAME_RENDER_COMMANDS_H

#ifdef __cplusplus
#include <nw/gfx/gfx_CommandUtil.h>
#include <nw/gfx/res/gfx_ResShape.h>

/* [INFERENCE] Addressed SDK draw-command operation. The wrapper, serialized
 * index-stream record and geometry-shader flag have their canonical types. */
extern "C" int func_00140298(nw::gfx::internal::CommandBufferInfo *buffer,
                              nw::gfx::res::ResIndexStreamData *stream,
                              bool hasGeometryShader);
#endif

#endif
