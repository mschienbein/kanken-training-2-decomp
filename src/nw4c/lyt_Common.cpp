/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_Common.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: lyt_Common.cpp
//
// Project: NintendoWare4Ctr

#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_DrawInfo.h>
#include <nw/lyt/lyt_GraphicsResource.h>
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Pane.h>
#include <nw/lyt/lyt_Layout.h>

#define ARRAY_LENGTH(a)   (sizeof(a) / sizeof((a)[0]))

namespace nw {
namespace lyt {
namespace {
    static const GLushort s_VertexIndex[] ={
        VERTEX_RT, VERTEX_LT, VERTEX_RB,
        VERTEX_LT, VERTEX_LB, VERTEX_RB,
    };

    static const int s_NumVertex = ARRAY_LENGTH(s_VertexIndex);
}
namespace internal{

using namespace nw::math;



TexCoordAry::TexCoordAry():   
    m_Cap(0),
    m_Num(0),
    m_pData(NULL)
{
}

void TexCoordAry::Free()
{
    if (m_pData)
    {
        const u32 coordNum = m_Cap;
        Layout::DeleteArray<math::VEC2>(&m_pData[0][0], VERTEX_MAX * coordNum);
        m_pData = 0;

        m_Cap = 0;
        m_Num = 0;
    }
}















}
}
}

