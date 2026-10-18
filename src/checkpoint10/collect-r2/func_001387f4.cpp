/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/res/gfx_ResShape.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected SetupDrawCommand definition, canonical resource
 * iterator/command-buffer helpers, and the evidenced addressed C call target.
 * Adjacency-TU hypothesis was split after incompatible SDK helper inlining and
 * profile evidence; no accepted context definitions are copied.
 */
#define NW_RELEASE
#include "GLES2/gl2extern.h"
#include "nw/gfx/gfx_CommandUtil.h"
#include "nw/gfx/res/gfx_ResShape.h"

/* INTEGRATOR: move to include/game/render_commands.h. Canonical command-buffer
 * and index-stream layouts replace the older private RenderStream and
 * RenderDrawCommand views. The third input is the native geometry-shader flag.
 */
extern "C" {
int func_00140298(nw::gfx::internal::CommandBufferInfo *,
                  nw::gfx::res::ResIndexStreamData *, bool);
}

namespace nw { namespace gfx { namespace res {
void ResPrimitive::SetupDrawCommand(bool hasGeometryShader)
{
    ResIndexStreamArray streams = GetIndexStreams();
    ResIndexStreamArray::iterator end = streams.end();
    for (ResIndexStreamArray::iterator stream = streams.begin(); stream != end; ++stream) {
        ResIndexStream indexStream = *stream;
        internal::CommandBufferInfo bufferInfo(indexStream.ref().m_CommandCache,
                                                indexStream.ref().m_CommandCacheSize);
        func_00140298(&bufferInfo, indexStream.ptr(), hasGeometryShader);
    }
    ref().m_Flags |= FLAG_COMMAND_HAS_BEEN_SETUP;
}
} } }
