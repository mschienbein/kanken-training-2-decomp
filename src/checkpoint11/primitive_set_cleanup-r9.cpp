/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/gfx/res/gfx_ResShape.cpp,
 * ResPrimitiveSet::Cleanup, ResPrimitive::Cleanup and DeleteBuffer.
 * Replaces w21d's inferred animation-resource types with canonical primitives.
 */
#include <preinclude.h>
#include "game/geometry_resources.h"
#include "game/render_state.h"
#include "game/screen.h"

static void delete_buffer(GLuint id, nw::gfx::res::ResIndexStream stream)
{
    /* ORIGINAL BUG: even a null index-stream view is dereferenced here. */
    unsigned area = stream.ref().m_MemoryArea;
    if (area != nw::gfx::res::ResIndexStreamData::AREA_NO_MALLOC) {
        void *address = reinterpret_cast<void *>(stream.GetLocationAddress());
        data_002429f8(area, 0x102, id, address);
        stream.SetLocationAddress(static_cast<unsigned>(0));
    }
}

inline void nw::gfx::res::ResPrimitive::Cleanup()
{
    using namespace nw::gfx;
    unsigned count = this->GetIndexStreamsCount();
    GLuint *buffers = this->GetBufferObjects();
    for (unsigned i = 0; i < count; ++i) {
        ResIndexStream stream = this->GetIndexStreams(i);
        if (stream.IsValid() && stream.ref().m_CommandCache) {
            this->ref().m_CommandAllocator->Free(stream.ref().m_CommandCache);
            stream.ref().m_CommandCache = NULL;
            stream.ref().m_CommandCacheSize = 0;
        }
        delete_buffer(buffers[i], stream);
    }
    func_0019cecc(buffers, sizeof(*buffers) * count);
    this->ref().m_CommandAllocator = NULL;
    this->ref().m_Flags &= ~ResPrimitive::FLAG_COMMAND_HAS_BEEN_SETUP;
}

extern "C" void func_001c78f8(nw::gfx::res::ResPrimitiveSet *primitiveSet)
{
    using namespace nw::gfx;
    ResPrimitiveArray primitives = primitiveSet->GetPrimitives();
    for (ResPrimitiveArray::iterator it = primitives.begin(); it != primitives.end(); ++it)
        (*it).Cleanup();
}
