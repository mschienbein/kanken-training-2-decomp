/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/res/gfx_ResShape.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-25: select SetupDrawCommand, use the canonical resource
 * iterator/command-buffer helpers, and publish the addressed SDK call contract
 * in game/render_commands.h. No accepted context definitions are copied.
 */
#define NW_RELEASE
#include "GLES2/gl2extern.h"
#include "game/render_commands.h"

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
