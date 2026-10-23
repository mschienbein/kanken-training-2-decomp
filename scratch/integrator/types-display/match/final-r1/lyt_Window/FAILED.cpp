/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_Window.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: lyt_Window.cpp
//
// Project: NintendoWare4Ctr

#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_DrawInfo.h>
#include <nw/lyt/lyt_GraphicsResource.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Window.h>
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_ResourceAccessor.h>

namespace nw{
namespace lyt{
namespace local{
namespace{

struct Rect
{
    f32 x, y, w, h;

    

    
};

struct TextureFlipInfo
{
    u8      coords[VERTEX_MAX][2];
    u8      idx[2];
};

/* Window Inlines */





















}
}

/* Window::Frame */



/* Window */































void Window::SetVtxColor(u32 idx,ut::Color8 value)
{
    m_Content.vtxColors[idx] = value;
}



void Window::SetVtxColorElement(u32 idx, u8 value)
{
    internal::SetVtxColorElement(m_Content.vtxColors, idx, value);
}



























}
}

