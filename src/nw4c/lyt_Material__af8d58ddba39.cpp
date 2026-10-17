/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_Material.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: lyt_Material.cpp
//
// Project: NintendoWare4Ctr

#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_DrawInfo.h>
#include <nw/lyt/lyt_GraphicsResource.h>
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Resources.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_ResourceAccessor.h>
#include <nw/lyt/lyt_Util.h>

namespace nw{
namespace lyt{
namespace{

const u32 DefaultBlackColor = 0x00000000;
const u32 DefaultWhiteColor = 0xffffffff;















struct TexEnvSetting
{
    u8 /*TevMode*/ m_ModeRgb;
    u8 /*TevMode*/ m_ModeAlpha;
    u8 /*TevSrc*/ m_SrcRgb0;
    u8 /*TevSrc*/ m_SrcRgb1;
    u8 /*TevSrc*/ m_SrcRgb2;
    u8 /*TevSrc*/ m_SrcAlpha0;
    u8 /*TevSrc*/ m_SrcAlpha1;
    u8 /*TevSrc*/ m_SrcAlpha2;
    u8 /*TevOpRgb*/ m_OpRgb0;
    u8 /*TevOpRgb*/ m_OpRgb1;
    u8 /*TevOpRgb*/ m_OpRgb2;
    u8 /*TevOpAlp*/ m_OpAlpha0;
    u8 /*TevOpAlp*/ m_OpAlpha1;
    u8 /*TevOpAlp*/ m_OpAlpha2;
    u8 /*TevScale*/ m_ScaleRgb;
    u8 /*TevScale*/ m_ScaleAlpha;

    
};

}

















































void Material::SetColorElement(u32 colorType, u8 value)
{
    if (colorType < ANIMTARGET_MATCOLOR_MAX)
    {

        const u32 idx = (colorType - ANIMTARGET_MATCOLOR_BUFFER_R) / 4;
        switch ((colorType - ANIMTARGET_MATCOLOR_BUFFER_R) % 4)
        {
        case 0: 
            this->m_Colors[idx].r = value; break;
        case 1: 
            this->m_Colors[idx].g = value; break;
        case 2: 
            this->m_Colors[idx].b = value; break;
        case 3: 
            this->m_Colors[idx].a = value; break;
        }
    }
}





















}
}

