/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/res/gfx_ResVertex.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select assigned definitions; use canonical curated headers,
 * evidenced numeric allocation selectors, and address-bound retained calls.
 * GNU GPL version 3; see src/nw4c/LICENSE.txt.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResVertex.h>
#include <game/render_state.h>
namespace nw { namespace gfx { namespace res {
/* INTEGRATOR: move this existing src/wave2/func_0011d260.c contract
 * to include/game/render_state.h; the target addresses the callback itself. */
extern "C" { extern void (*data_002429f8)(unsigned module, unsigned kind,
    unsigned id, void *memory); }
static void DeleteBuffer(u32 bufferID, ResVertexStreamBase stream)
{
    GLuint area = stream.ref().m_MemoryArea;
    if (area != ResVertexStreamBaseData::AREA_NO_MALLOC) {
        void *address = reinterpret_cast<void *>(stream.GetLocationAddress());
        data_002429f8(area, 0x102u, bufferID, address);
        stream.SetLocationAddress(static_cast<u32>(NULL));
    }
}
inline void ResVertexStream::Cleanup()
{
    GLuint bufferID = this->GetBufferObject();
    if (bufferID == 0) return;
    DeleteBuffer(bufferID, *this);
    this->SetBufferObject(0);
}
inline void ResInterleavedVertexStream::Cleanup()
{
    GLuint bufferID = this->GetBufferObject();
    if (bufferID == 0) return;
    DeleteBuffer(bufferID, *this);
    this->SetBufferObject(0);
}
void ResVertexAttribute::Cleanup()
{
    NW_ASSERT(this->IsValid());
    if (this->GetFlags() & ResVertexAttributeData::FLAG_VERTEX_PARAM) {
        return;
    } else if (this->GetFlags() & ResVertexAttributeData::FLAG_INTERLEAVE) {
        nw::ut::ResStaticCast<ResInterleavedVertexStream>(*this).Cleanup();
    } else {
        nw::ut::ResStaticCast<ResVertexStream>(*this).Cleanup();
    }
}
} } }
