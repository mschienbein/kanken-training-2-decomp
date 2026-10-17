/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_TexResource.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "lyt_Arc.h"
#include "lyt_Resources.h"
#include "lyt_Types.h"

namespace nw{
namespace lyt{

class TexResource
{
public:
    TexResource(): 
        m_pTop(NULL), 
        m_pImage(NULL), 
        m_pImageSize(NULL) 
    {}
    TexResource(void* pTexRes, u32 size)
    {
        this->Set(pTexRes, size);
    }

    explicit TexResource(ARCFileInfo& fileInfo)
    {
        this->Set(fileInfo);
    }

    bool IsValid() const {return m_pTop != NULL;}
    bool Set(void* pTexRes, u32 size);
    bool Set(ARCFileInfo& fileInfo)
    {
        return this->Set(
            ARCGetStartAddrInMem(&fileInfo),
            ARCGetLength(&fileInfo));
    }

    const void* GetImageAddress() const {return m_pTop;}
    void* GetImageAddress() {return m_pTop;}
    u32 GetImageSize() const {return m_pImageSize->imageSize;}
    lyt::TexFormat GetFormat() const {return static_cast<lyt::TexFormat>(m_pImage->format);}

    bool Is4bitFormat() const
    {
        return m_pImage->format == lyt::TEXFORMAT_L4
            || m_pImage->format == lyt::TEXFORMAT_A4;
    }
    u16 GetWidth() const {return m_pImage->width;}
    u16 GetHeight() const {return m_pImage->height;}
    MemArea GetImageArea() const
    {
        return static_cast<lyt::MemArea>(internal::GetBits(this->m_pImage->flag, lyt::IMAGEFLAG_AREA_POS,lyt::IMAGEFLAG_AREA_LEN));
    }
    void SetImageArea(lyt::MemArea area)
    {
        m_pImage->flag = internal::SetBits(this->m_pImage->flag, lyt::IMAGEFLAG_AREA_POS, lyt::IMAGEFLAG_AREA_LEN,static_cast<u8>(area));
    }

    res::Image* GetResImage() {return m_pImage;}
    const res::Image* GetResImage() const {return m_pImage;}

protected:
    void* m_pTop;
    res::Image* m_pImage;
    res::ImageSize* m_pImageSize;
};

}
}

