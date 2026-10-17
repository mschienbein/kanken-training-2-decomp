/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/ut/ut_CmdCache.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../../GLES2/gl2.h"
#include "../types.h"

namespace nw {
namespace ut {
namespace internal {

class CmdCache
{
public:
    CmdCache();
    void Init(void* buffer,u32 size,bool isCopy  = true);
    void Use() const;
    void Add(const u32*  command,u32 size);
    void RoundUp(u8 align);
    void Clear() { this->m_CmdBufferSize = 0; }
    void* GetAddr() const { return this->m_CmdBuffer; }
    GLsizei GetSize() const { return this->m_CmdBufferSize; }
    void Dump(bool asF32 = false,bool showMnemonic = false) const;
    static void Dump(const void* from,const void* to,bool asF32 = false,bool showMnemonic = false);
private:
    static void DumpCommon(const void* from,const void* to,bool asF32,bool showMnemonic);
    u8* m_CmdBuffer;
    GLsizei m_CmdBufferSize;
    GLsizei m_CmdMaxBufferSize;
    bool m_IsCopy;
};

} // namespace internal
} // namespace ut
} // namespace nw

