#ifndef GAME_RENDER_COMMANDS_H
#define GAME_RENDER_COMMANDS_H

#ifdef __cplusplus
#include <nw/gfx/gfx_CommandUtil.h>
#include <nw/gfx/res/gfx_ResShape.h>

/* tu491's exact ResPrimitive::SetupDrawCommand caller supplies the canonical
 * native command-buffer and index-stream objects and a geometry-shader bool.
 * The recovered method/type names remain qualified public-source identities. */
extern "C" int func_00140298(nw::gfx::internal::CommandBufferInfo *buffer,
                            nw::gfx::res::ResIndexStreamData *stream,
                            bool has_geometry_shader);
#endif

#endif
