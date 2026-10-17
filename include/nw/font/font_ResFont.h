/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/font/font_ResFont.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain public layouts; normalize assertion includes and remove unused SDK/module and graphics-type dependencies.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <nw/types.h>
#include <nw/font/font_ResFontBase.h>

namespace nw{
namespace font{
class ResFont : public ResFontBase
{
public:
    ResFont();
    virtual ~ResFont();

    void RemoveResource();
    void* SetDrawBuffer(void* buffer);
    void* SetResource(void* bfnt);
    static u32 GetDrawBufferSize(const void* bfnt);
    void* GetDrawBuffer() {return GetTextureObjectsBufferPtr();}

    using ResFontBase::IsManaging;
};

}
}

