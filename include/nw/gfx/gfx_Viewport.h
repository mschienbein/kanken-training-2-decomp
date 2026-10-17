/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_Viewport.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../ut/ut_Rect.h"
#include "gfx_IRenderTarget.h"

namespace nw{
namespace gfx{

class Viewport
{
public:

    Viewport(): 
        m_DepthNear(0.f),
        m_DepthFar(1.f) {}

    Viewport(float x,float y,float width,float height,float near, float far ): 
        m_Rect( x, y, x + width, y + height ),
        m_DepthNear(near),
        m_DepthFar(far) {}

    Viewport(nw::ut::Rect rect,float near,float far ): 
        m_Rect( rect ),
        m_DepthNear(near),
        m_DepthFar(far) {}

    Viewport(IRenderTarget* renderTarget,float near,float far): 
        m_DepthNear(near),
        m_DepthFar(far) 
    {
       NW_NULL_ASSERT(renderTarget);

       f32 width = static_cast<f32>(renderTarget->GetDescription().width);
       f32 height = static_cast<f32>(renderTarget->GetDescription().height);
       
       this->m_Rect.SetOriginAndSize(0.f, 0.f, width, height);
    }

    Viewport(const Viewport& v): 
        m_Rect(v.m_Rect),
        m_DepthNear(v.m_DepthNear),
        m_DepthFar(v.m_DepthFar) {}

    ~Viewport() {}

    void SetDepthRange(f32 near, f32 far)
    {
        this->m_DepthNear = near;
        this->m_DepthFar  = far;
    }

    void SetBound(const nw::ut::Rect& bound)
    {
        this->m_Rect = bound;
    }

    void SetBound(float x, float y, float width, float height)
    {
        this->m_Rect.SetOriginAndSize(x,y,width,height );
    }

    void SetBound(IRenderTarget* renderTarget)
    {
        NW_NULL_ASSERT(renderTarget);

        this->m_Rect.SetOriginAndSize(0.f, 0.f,static_cast<f32>(renderTarget->GetDescription().width),static_cast<f32>(renderTarget->GetDescription().height));
    }

    const nw::ut::Rect& GetBound() const { return m_Rect; }

    f32 GetDepthNear() const { return m_DepthNear; }

    f32 GetDepthFar() const { return m_DepthFar; }

private:
    nw::ut::Rect m_Rect;
    f32 m_DepthNear;
    f32 m_DepthFar;
};

}
}

