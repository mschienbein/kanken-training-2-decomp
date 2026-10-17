/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_TexMap.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../types.h"
#include "lyt_Types.h"
#include "lyt_Drawer.h"

namespace nw{
namespace lyt{
namespace res{
    struct Image;
}

class TextureInfo;

class TexMap
{
public:
    TexMap();
    TexMap(const TextureInfo& textureInfo);
    void ResetU32Info();

    /* Inlines */

    void Set(const TextureInfo& src)
    {
        this->m_TexObject = src.GetTextureObject();
        this->m_PhysicalAddress = src.GetPhysicalAddress();
        this->m_Width = src.GetSize().width;
        this->m_Height = src.GetSize().height;
        this->m_RealWidth = src.GetRealSize().width;
        this->m_RealHeight = src.GetRealSize().height;
        this->m_Bits.format = src.GetFormat();
        this->ResetU32Info();
    }
    
    void SetWrapMode(TexWrap wrapS, TexWrap wrapT)
    {
        m_Bits.wrapS = wrapS;
        m_Bits.wrapT = wrapT;

        this->ResetU32Info();
    }

    void SetFilter(TexFilter minFlt, TexFilter magFlt)
    {
        m_Bits.minFilter = minFlt;
        m_Bits.magFilter = magFlt;

        this->ResetU32Info();
    }
    
    const TexSize GetSize() const
    {
        return TexSize(this->m_Width, this->m_Height);
    }

    ushort GetHeight() const { return m_Height; }
    ushort GetWidth() const {return m_Width; }
    ushort GetRealWidth() const {return m_RealWidth;}
    ushort GetRealHeight() const {return m_RealHeight;}

    TexFormat GetFormat() const {return TexFormat(m_Bits.format);}

    TexFilter GetMinFilter() const {return TexFilter(m_Bits.minFilter);}
    TexFilter GetMagFilter() const {return TexFilter(m_Bits.magFilter);}

    TexWrap GetWrapModeS() const {return TexWrap(m_Bits.wrapS);}
    TexWrap GetWrapModeT() const {return TexWrap(m_Bits.wrapT);}

    u32 GetTextureObject() const { return m_TexObject; }

    uptr GetPhysicalAddress() const { return m_PhysicalAddress; }

    u32 GetU32WrapFilter() const { return mu32WrapFilter; }
    u32 GetU32Format() const { return mu32Format; }
    u32 GetU32WidthHeight() const { return mu32WidthHeight; }

protected:
    struct Bits
    {
        u32 wrapS: 2;
        u32 wrapT: 2;
        u32 minFilter: 3;
        u32 magFilter: 1;
        u32 format: 4;
    };

    u32 m_TexObject;
    uptr m_PhysicalAddress;
    u16 m_Width;
    u16 m_Height;
    u16 m_RealWidth;
    u16 m_RealHeight;
    Bits m_Bits;
    u32 mu32WrapFilter;
    u32 mu32WidthHeight;
    u32 mu32Format;
};

}
}

