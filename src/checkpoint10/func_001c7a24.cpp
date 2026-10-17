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
/* INTEGRATOR: move to include/game/vertex_initialization.h. Native
 * resource-wrapper value replaces the earlier pointer-only draft contract. */
extern "C" { void func_001c73cc(u32 bufferID, ResVertexStreamBase stream, u32 loadFlag); }
inline void ResVertexStream::Setup()
{
    if (this->ref().m_BufferObject) return;
    ResVertexStreamData *dataPtr = this->ptr();
    u32 bufferID = reinterpret_cast<u32>(dataPtr);
    this->ref().m_BufferObject = bufferID;
    if (this->GetLocationAddress() != NULL) {
        this->ref().m_MemoryArea = ResVertexStreamBaseData::AREA_NO_MALLOC;
        return;
    }
    int loadFlag = this->GetLocationFlag();
    if (loadFlag == 0) loadFlag = 0x02010000;
    func_001c73cc(bufferID, *this, loadFlag);
}
inline void ResInterleavedVertexStream::Setup()
{
    if (this->ref().m_BufferObject) return;
    u32 bufferID = reinterpret_cast<u32>(this->ptr());
    this->ref().m_BufferObject = bufferID;
    if (this->GetLocationAddress() != NULL) {
        this->ref().m_MemoryArea = ResVertexStreamBaseData::AREA_NO_MALLOC;
        return;
    }
    int loadFlag = this->GetLocationFlag();
    if (loadFlag == 0) loadFlag = 0x02010000;
    func_001c73cc(bufferID, *this, loadFlag);
}
extern "C" void func_001c7a24(ResVertexAttribute *attribute)
{
    NW_ASSERT(attribute->IsValid());
    if (attribute->GetFlags() & ResVertexAttributeData::FLAG_VERTEX_PARAM) {
        return;
    } else if (attribute->GetFlags() & ResVertexAttributeData::FLAG_INTERLEAVE) {
        nw::ut::ResStaticCast<ResInterleavedVertexStream>(*attribute).Setup();
    } else {
        nw::ut::ResStaticCast<ResVertexStream>(*attribute).Setup();
    }
}
} } }
