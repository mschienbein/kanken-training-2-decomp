/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_OffScreenBuffer.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#include "gfx_IRenderTarget.h"
#include "../ut/ut_Preprocessor.h"
#include "res/gfx_ResTexture.h"
#include "gfx_FrameBuffer.h"

namespace nw{
namespace os{
    class IAllocator;
}
namespace gfx
{

class OffScreenBuffer : public IRenderTarget
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(OffScreenBuffer);

public:
    NW_UT_RUNTIME_TYPEINFO;

    virtual const Description& GetDescription() const { return m_Description; }
    virtual const FrameBufferObject& GetBufferObject() const { return m_BackBufferObject; }

private:
    OffScreenBuffer(nw::os::IAllocator* pAllocator, const Description& description, ResPixelBasedTexture resTexture);
    virtual ~OffScreenBuffer();

    FrameBufferObject m_BackBufferObject;
    void* m_ActivateCommand;
    Description m_Description;
    ResTexture m_Texture;

    friend class IRenderTarget;
};

}
}

