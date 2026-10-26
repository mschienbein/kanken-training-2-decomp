#ifndef GAME_RENDER_CONTEXT_H
#define GAME_RENDER_CONTEXT_H

#include <nw/gfx/gfx_RenderContext.h>

/* [INFERENCE] Address-named renderer endpoints use the canonical native owner
 * and resource-wrapper types. MaterialHash is the actual embedded cache, not a
 * second eleven-word view of that object. */
extern "C" {
void func_0013862c(nw::gfx::ShaderProgram *shader,
    nw::gfx::ResShaderProgramDescription description);
void func_001384a0(nw::gfx::RenderContext *context);
void func_00191c94(nw::gfx::RenderContext::MaterialHash *hash, unsigned flags);
}

#endif
