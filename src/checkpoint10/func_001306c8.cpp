/* SPDX-License-Identifier: GPL-3.0-only
 * Canonical NintendoWare4Ctr resource and rendering interfaces.
 * [INFERENCE] Native member identity recovered from the matching operation;
 * no runtime renderer or serialized resource shadow view is retained.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_RenderContext.h>
#include <nw/gfx/gfx_ShaderProgram.h>

namespace nw { namespace gfx {
void RenderContext::RenderPrimitive(ResPrimitive primitive)
{
    ShaderProgram *shaderProgram = GetShaderProgram();
    shaderProgram->FlushUniform();
    if (!(primitive.GetFlags() & ResPrimitive::FLAG_COMMAND_HAS_BEEN_SETUP))
        primitive.SetupDrawCommand(GetShaderProgram()->GetActiveDescription().GetGeometryShaderObject() != 0);
    ResIndexStreamArray streams = primitive.GetIndexStreams();
    ResIndexStreamArray::iterator end = streams.end();
    for (ResIndexStreamArray::iterator stream = streams.begin(); stream != end; ++stream) {
        ResIndexStream indexStream = *stream;
        if (indexStream.IsVisible())
            internal::NWUseCmdlist(indexStream.ref().m_CommandCache,
                                  indexStream.ref().m_CommandCacheSize);
    }
}
} }
