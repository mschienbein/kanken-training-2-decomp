/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_Types.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain only the resource-accessor texture/size/key interfaces; omit unrelated layout, animation, and SDK math declarations. Retained layouts and inline bodies unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#pragma once
#include <nw/types.h>
#include <nw/ut/ut_ResTypes.h>

namespace nw { namespace lyt {
enum TexFormat
{
    TEXFORMAT_L8,
    TEXFORMAT_A8,
    TEXFORMAT_LA4,
    TEXFORMAT_LA8,
    TEXFORMAT_HILO8,
    TEXFORMAT_RGB565,
    TEXFORMAT_RGB8,
    TEXFORMAT_RGB5A1,
    TEXFORMAT_RGBA4,
    TEXFORMAT_RGBA8,
    TEXFORMAT_ETC1,
    TEXFORMAT_ETC1A4,
    TEXFORMAT_L4,
    TEXFORMAT_A4,
    TEXFORMAT_MAX
};

struct Size
{
    Size(): width(0), height(0) {}
    Size(f32 aWidth, f32 aHeight): width(aWidth), height(aHeight) {}
    Size(const Size& other): width(other.width), height(other.height) {}

    ut::ResF32 width;
    ut::ResF32 height;
};

inline bool operator==(const Size& a, const Size& b)
{
    return a.width == b.width && a.height == b.height;
}

struct TexSize
{
    TexSize(): width(0), height(0) {}
    TexSize(u16 aWidth, u16 aHeight): width(aWidth), height(aHeight) {}
    TexSize(const TexSize& other): width(other.width), height(other.height) {}

    operator const Size() const { return Size(this->width, this->height); }

    u16 width;
    u16 height;
};

class TextureInfo
{
public:
    static const u32 INVALID = 0;

    TextureInfo(): m_TexObject(INVALID), m_PhysicalAddress(NULL) {}
    TextureInfo(const TextureInfo& src) { this->Set(src); }
    TextureInfo(u32 texObject, uptr physicalAddress, const TexSize& size, const TexSize& realSize, TexFormat format)
    {
        this->Set(texObject, physicalAddress, size, realSize, format);
    }

    void Set(const TextureInfo& src) { *this = src; }

    void Set(u32 texObject, uptr physicalAddress, const TexSize& size, const TexSize& realSize, TexFormat format)
    {
        m_TexObject       = texObject;
        m_PhysicalAddress = physicalAddress;
        m_Size            = size;
        m_RealSize        = realSize;
        m_Format          = format;
    }

    u32            GetTextureObject()   const { return m_TexObject; }
    const TexSize& GetSize()            const { return m_Size; }
    const TexSize& GetRealSize()        const { return m_RealSize; }
    uptr           GetPhysicalAddress() const { return m_PhysicalAddress; }
    TexFormat      GetFormat()          const { return TexFormat(m_Format); }

    bool IsValid() const
    {
        return m_PhysicalAddress != 0;
    }

protected:
    static bool IsPowerOfTwo(u32 value) { return (value & ~(value - 1)) == value; }

protected:
    u32     m_TexObject;
    uptr    m_PhysicalAddress;
    TexSize m_Size;
    TexSize m_RealSize;
    u8      m_Format;
};

typedef const void* FontKey;
typedef const void* TextureKey;

} }
