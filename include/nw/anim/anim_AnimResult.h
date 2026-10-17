/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/anim/anim_AnimResult.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../types.h"
#include "../ut/ut_Flag.h"
#include "../gfx/gfx_CalculatedTransform.h"

namespace nw   {
namespace anim {

class AnimResult
{
public:
    enum Flags
    {
        FLAG_VALID_COMPONENT0_SHIFT = 0,
        FLAG_VALID_COMPONENT1_SHIFT = 1,
        FLAG_VALID_COMPONENT2_SHIFT = 2,
        FLAG_VALID_COMPONENT3_SHIFT = 3,
        FLAG_CONVERTED_SHIFT = 30,
        FLAG_VALID_COMPONENT0 = 0x1 << FLAG_VALID_COMPONENT0_SHIFT,

        FLAG_VALID_COMPONENT1 = 0x1 << FLAG_VALID_COMPONENT1_SHIFT,

        FLAG_VALID_COMPONENT2 = 0x1 << FLAG_VALID_COMPONENT2_SHIFT,

        FLAG_VALID_COMPONENT3 = 0x1 << FLAG_VALID_COMPONENT3_SHIFT,

        FLAG_CONVERTED = 0x1 << FLAG_CONVERTED_SHIFT
    };

    enum
{ MAX_COMPONENTS = 4 };

    AnimResult(): 
        m_Flags(0) {}

    virtual ~AnimResult() {}

    bit32 GetFlags() const { return m_Flags; }

    void SetFlags(bit32 flags) { m_Flags = flags; }

    bool IsEnabledFlags(bit32 flags) const { return nw::ut::CheckFlag(m_Flags, flags); } 

    void EnableFlags(bit32 flags) { m_Flags = nw::ut::EnableFlag(m_Flags, flags); }

    void DisableFlags(bit32 flags) { m_Flags = nw::ut::DisableFlag(m_Flags, flags); }

    void EnableFlags(bit32 flags, bool enable)
    {
        if (enable)
        {
            m_Flags |= flags;
        }
        else{
            m_Flags &= ~flags;
        }
    }

    void ResetFlags() { m_Flags = 0; }

    const void* GetValueBuffer() const { return m_ValueBuf; }

    void* GetValueBuffer() { return m_ValueBuf; }

    int GetOffsetToValueBuffer() const
    {
        return reinterpret_cast<const u8*>(GetValueBuffer()) -
            reinterpret_cast<const u8*>(this);
    }

private:
    bit32 m_Flags;

    float m_ValueBuf[(sizeof(gfx::CalculatedTransform) + sizeof(float) - 1) / sizeof(float)];
};

}
}

