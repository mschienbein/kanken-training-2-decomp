/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/font/font_Types.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../types.h"
#include "../ut/ut_Color.h"
#include "font_DrawerTypes.h"

namespace nw {
namespace font {
namespace internal {

enum
{
    VERTEX_ATTR_POS,
    VERTEX_ATTR_POS_Z,
    VERTEX_ATTR_COLOR,
    VERTEX_ATTR_TEXCOORD,

    VERTEX_ATTR_NUM
};

enum
{
    POS_X,
    POS_Y,

    POS_NUM
};

enum
{
    COLOR_R,
    COLOR_G,
    COLOR_B,
    COLOR_A,

    COLOR_NUM
};

enum
{
    TEXCOORD_X,
    TEXCOORD_Y,

    TEXCOORD_NUM
};

enum
{
    TRIFAN_VTX_RT,
    TRIFAN_VTX_LT,
    TRIFAN_VTX_LB,
    TRIFAN_VTX_RB,

    TRIFAN_VTX_MAX
};

enum
{
    LOC_PROJECTION,
    LOC_MODELVIEW,

    LOC_FRAGMENTLIGHTING_ENABLED,
    LOC_TEXTURE0_SAMPLERTYPE,
    LOC_FOG_MODE,
    LOC_FRAGOPERATION_ENABLEALPHATEST,

    LOC_MAX
};

enum
{
    TCLOC_SRCRGB,
    TCLOC_SRCALPHA,
    TCLOC_OPERANDRGB,
    TCLOC_OPERANDALPHA,
    TCLOC_COMBINERGB,
    TCLOC_COMBINEALPHA,
    TCLOC_SCALERGB,
    TCLOC_SCALEALPHA,
    TCLOC_CONSTRGBA,

    TCLOC_MAX
};

enum
{
    TEXENV_3,
    TEXENV_4,
    TEXENV_5,

    TEXENV_MAX
};

enum TextColor
{
    TEXTCOLOR_START, 
    TEXTCOLOR_END,

    TEXTCOLOR_MAX
};

struct VertexAttribute
{
    f32 pos[POS_NUM];
    ut::Color8 color;
    f32 tex[TEXCOORD_NUM];
};

}
}
}

