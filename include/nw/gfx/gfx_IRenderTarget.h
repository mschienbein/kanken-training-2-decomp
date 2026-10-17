/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_IRenderTarget.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: normalize standalone includes/UTF-8; omit unevidenced SDK
 * memory enumerators; declare the default constructor only. Public fields,
 * enum type and all remaining algorithms retain their upstream order/types.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_GfxObject.h"
#include "../ut/ut_RuntimeTypeInfo.h"
#include "gfx_Common.h"
#include "gfx_FrameBuffer.h"
#include "res/gfx_ResTexture.h"

#include "../../gles2/gl2.h"
#include "../../gles2/gl2extern.h"
#include "../../nn/gx/CTR/gx_CTR.h"

namespace nw{
namespace gfx{
namespace res{
    class ResTexture;
}

enum RenderColorFormat
{
    RENDER_COLOR_FORMAT_NONE,
    RENDER_COLOR_FORMAT_RGBA8 = GL_RGBA8_OES,
    RENDER_COLOR_FORMAT_RGB8 = GL_RGB8_OES,
    RENDER_COLOR_FORMAT_RGBA4 = GL_RGBA4,
    RENDER_COLOR_FORMAT_RGB5_A1 = GL_RGB5_A1,
    RENDER_COLOR_FORMAT_RGB565 = GL_RGB565,
    RENDER_COLOR_FORMAT_COUNT
};

enum RenderDepthFormat
{
    RENDER_DEPTH_FORMAT_NONE,
    RENDER_DEPTH_FORMAT_16 = GL_DEPTH_COMPONENT16,
    RENDER_DEPTH_FORMAT_24 = GL_DEPTH_COMPONENT24_OES,
    RENDER_DEPTH_FORMAT_24_STENCIL8 = GL_DEPTH24_STENCIL8_EXT,
    RENDER_DEPTH_FORMAT_COUNT
};



enum GraphicsMemoryArea
{
    MEMORY_AREA_NONE
    /* Unproved SDK memory selector values deliberately omitted. */
};

enum ShadowKind
{
    SHADOW_KIND_NONE,
    SHADOW_KIND_TEXTURE,
    SHADOW_KIND_CUBE
};

class IRenderTarget : public GfxObject
{
public:
    NW_UT_RUNTIME_TYPEINFO;

    struct Description
    {
        s32 width;
        s32 height;
        RenderColorFormat colorFormat;
        RenderDepthFormat depthFormat;
        GraphicsMemoryArea colorArea;
        GraphicsMemoryArea depthArea;
        u32 colorAddress;
        u32 depthAddress;
        ShadowKind shadowKind;

        /* Declaration-only: the default memory selectors remain unproved. */
        Description();
    };

    class Builder
    {
    public:
        Builder& Described(const Description& description)
        {
            m_Description = description;
            return *this;
        }

        Builder& BufferSize(s32 width, s32 height)
        {
            m_Description.width = width;
            m_Description.height = height;
            return *this;
        }

        Builder& Width(s32 width) { m_Description.width = width; return *this; }

        Builder& Height(s32 height) { m_Description.height = height; return *this; }

        Builder& ColorFormat(RenderColorFormat format)
        {
            m_Description.colorFormat = format;
            return *this;
        }

        Builder& DepthFormat(RenderDepthFormat format)
        {
            m_Description.depthFormat = format;
            return *this;
        }

        Builder& ColorArea(GraphicsMemoryArea memory)
        {
            m_Description.colorArea = memory;
            return *this;
        }

        Builder& DepthArea(GraphicsMemoryArea memory)
        {
            m_Description.depthArea = memory;
            return *this;
        }

        Builder& ColorAddress(u32 colorAddress)
        {
            m_Description.colorAddress = colorAddress;
            return *this;
        }

        Builder& DepthAddress(u32 depthAddress)
        {
            m_Description.depthAddress = depthAddress;
            return *this;
        }

        IRenderTarget* Create(nw::os::IAllocator* allocator);
        
    private:
        Description m_Description;
    };
    static IRenderTarget* CreateOffScreenBuffer(nw::os::IAllocator* allocator, ResTexture resTexture);
    virtual const Description& GetDescription() const = 0;
    virtual const FrameBufferObject& GetBufferObject() const = 0;
    
protected:

    IRenderTarget(nw::os::IAllocator* allocator) : GfxObject(allocator) {}
    virtual ~IRenderTarget() {}
};

}
}

