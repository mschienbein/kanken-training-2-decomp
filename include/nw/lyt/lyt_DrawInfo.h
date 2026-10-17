/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_DrawInfo.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../math/math_Types.h"
#include "../font/font_WideTextWriter.h"
#include "../ut/ut_Rect.h"

namespace nw{
namespace lyt{

class GraphicsResource;
class Layout;

class DrawInfo
{
public:
    DrawInfo();
    virtual ~DrawInfo();

    const math::MTX34&  GetViewMtx() const {return m_ViewMtx;}
    void SetViewMtx(const math::MTX34& value) {m_ViewMtx = value;}
    bool IsInfluencedAlpha() const {return m_Flag.influencedAlpha;}
    void SetInfluencedAlpha(bool bEnable) {m_Flag.influencedAlpha = bEnable;}
    bool IsLocationAdjust() const {return m_Flag.locationAdjust;}
    void SetLocationAdjust(bool bEnable) {m_Flag.locationAdjust = bEnable;}
    const math::VEC2& GetLocationAdjustScale() const { return m_LocationAdjustScale;}
    void SetLocationAdjustScale(const math::VEC2& scale) {m_LocationAdjustScale = scale;}
    f32 GetGlobalAlpha() const {return m_GlobalAlpha;}
    void SetGlobalAlpha(f32 alpha) {m_GlobalAlpha = alpha;}
    bool IsInvisiblePaneCalculateMtx() const {return m_Flag.invisiblePaneCalculateMtx;}
    void SetInvisiblePaneCalculateMtx(bool bEnable) {m_Flag.invisiblePaneCalculateMtx = bEnable;}
    GraphicsResource* GetGraphicsResource() const {return m_pGraphicsResource;}

    void SetProjectionMtx(const nw::math::MTX44& mtx);

    void SetLayout(Layout* pLayout) const {m_pLayout = pLayout;}

    Layout* GetLayout() const { return m_pLayout; }
    const nw::math::MTX44& GetProjectionMtx() const {return m_ProjMtx;}
protected:
    math::MTX44 m_ProjMtx;
    math::MTX34 m_ViewMtx;
    math::VEC2 m_LocationAdjustScale;
    f32 m_GlobalAlpha;
    GraphicsResource* m_pGraphicsResource;
    mutable Layout* m_pLayout;

    struct Flag
    {
        u8 influencedAlpha : 1;
        u8 locationAdjust : 1;
        u8 invisiblePaneCalculateMtx : 1;
    };
    Flag m_Flag;
};

}
}

