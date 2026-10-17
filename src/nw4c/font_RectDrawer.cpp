/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/font/font_RectDrawer.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: font_RectDrawer.cpp
//
// Project: NintendoWare4Ctr

#include <GLES2/gl2.h>
#include <GLES2/gl2extern.h>
#include <nw/types.h>
#include <nw/math/math_Types.h>
#include <nw/font/font_RectDrawer.h>
#include <nw/font/font_RectDrawerCommand.h>
#include <nw/font/font_DispStringBuffer.h>
#include <nn/gx/CTR/gx_CommandAccess.h>
#include <nn/gx/CTR/gx_PicaAll.h>

extern int* __cb_current_command_buffer;

namespace nw{
namespace font{
namespace{

const u32 DRAW_MODE2_DEFAULT  = 0;
const u32 DRAW_FUNC_DEFAULT   = 0;
const u32 DRAW_MODE0_DEFAULT  = 0;

const u32 DRAW_MODE0_ELEMENT  = 1;
const u32 DRAW_MODE2_ELEMENT  = PICA_DATA_DRAW_TRIANGLES;

const u32 s_DrawInitCommands[] =
{
    NW_FONT_COMMAND_SET_BLEND_DEFAULT,
    0x0,
    (PICA_REG_FRAGOP_ALPHA_TEST | 0x000F0000),
    (PICA_DATA_FOG_FALSE | (0 << 3) | (0 << 16)),
    (PICA_REG_GAS_FOG_MODE | (0xd << 16)),
    0,
    (PICA_REG_FRAG_LIGHT_EN0 | 0x000F0000),
    1,
    (PICA_REG_FRAG_LIGHT_EN1 | 0x000F0000),
    0x0000000f,
    (PICA_REG_COLOR_BUFFER_READ | 0x000F0000),
    0,
    (PICA_REG_TEXTURE0_LOD | 0x000F0000),
    0,
    (PICA_REG_TEXTURE1_LOD | 0x000F0000),
    0,
    (PICA_REG_TEXTURE2_LOD | 0x000F0000),
    1,
    (PICA_REG_TEXTURE0_SHADOW | 0x000F0000),
    (DRAW_MODE0_ELEMENT << 8),
    (PICA_REG_DRAW_MODE0 | (0x2 << 16)),
    (DRAW_FUNC_DEFAULT | (DRAW_MODE0_ELEMENT << 8)),
    (PICA_REG_DRAW_MODE1 | (0x3 << 16)),
    (DRAW_MODE2_ELEMENT << 8),
    (PICA_REG_VS_OUT_REG_NUM3 | (0x2 << 16)),
    1,
    (PICA_REG_COLOR_DEPTH_BUFFER_CLEAR1 | 0x000F0000),
    1,
    (PICA_REG_COLOR_DEPTH_BUFFER_CLEAR0 | 0x000F0000),
};



struct ShaderBinaryFileHeader
{
    u8      signature[4];
    u32     exeCount;
    u32     exeOffsetTop;
};

struct ShaderPackageHeader
{
    u8      signature[4];
    u8      version[2];
    u16     reserved0;
    u32     instOffset;
    u32     instCount;
    u32     swizzleOffset;
    u32     swizzleCount;
    u32     lineOffset;
    u32     lineCount;
    u32     stringOffset;
    u32     stringSize;
};

struct SwizzleInfo
{
    u32     value;
    u16     usedInfo;
    u16     reserve;
};

struct ExeImageHeader
{
    u8      signature[4];

    u8      version[2];
    u8      shaderType;
    u8      mergeOutputMapsDebug;

    u32     mainAddr;
    u32     endAddr;

    u16     inputMask;
    u16     outputMask;

    u8      geometryDataMode;
    u8      startIndex;
    u8      subdPatchSize;
    u8      constVertexNumber;

    u32     setupOffset;
    u32     setupCount;
    u32     labelOffset;
    u32     labelCount;
    u32     outmapOffset;
    u32     outmapCount;
    u32     bsymOffset;
    u32     bsymCount;
    u32     stringOffset;
    u32     stringSize;
};

struct SetupInfo
{
    u16     type;
    u16     index;
    u32     value[4];
};

struct OutmapInfo
{
    u16     type;
    u16     index;
    u16     mask;
    u16     reserve;
};

const u32 DRAWBEGIN_STATICCOMMAND_BYTES  = 656 + sizeof(s_DrawInitCommands) + sizeof(u32) * 2 * 1;

const u32 BURST_MAX = 0x80;

enum
{
    SHADEROUTMAPTYPE_POSITION,
    SHADEROUTMAPTYPE_QUATERNION,
    SHADEROUTMAPTYPE_COLOR,
    SHADEROUTMAPTYPE_TEXCOORD0,
    SHADEROUTMAPTYPE_TEXCOORD0W,
    SHADEROUTMAPTYPE_TEXCOORD1,
    SHADEROUTMAPTYPE_TEXCOORD2,
    SHADEROUTMAPTYPE_VIEW,

    SHADEROUTMAPTYPE_MAX
};























const u32 s_DrawFinalizeCommands[] ={
    (0x0 & 0xf),
    (PICA_REG_VS_FIXED_ATTR | (0xF << 16) | ((4 - 1) << 20) | 0x80000000),
    (u32)(0x3f000000),
    (u32)(0x00000000),
    (u32)(0x00000000),
    0x00000000,

    (DRAW_MODE0_DEFAULT << 8),
    (PICA_REG_DRAW_MODE0 | (0x2 << 16)),
    (0 | (DRAW_MODE0_DEFAULT << 8)),
    (PICA_REG_DRAW_MODE1 | (0x2 << 16)),
    (DRAW_MODE2_DEFAULT << 8),
    (PICA_REG_VS_OUT_REG_NUM3 | (0x2 << 16)),
    1,
    (PICA_REG_COLOR_DEPTH_BUFFER_CLEAR1 | 0x000F0000),
    1,
    (PICA_REG_COLOR_DEPTH_BUFFER_CLEAR0 | 0x000F0000),
};

const u32 DRAWEND_STATICCOMMAND_BYTES = 176 + sizeof(s_DrawFinalizeCommands);

}
















void RectDrawer::Initialize(void* vertexBuffer,void* commandBuffer,const void* shaderBinary,u32 size)
{
    this->InitializeCMD(vertexBuffer, commandBuffer, shaderBinary, size, false);
}

void RectDrawer::Finalize()
{
    m_CommandBuffer = NULL;
    m_VertexBufferArray = NULL;
}













void RectDrawer::SetParallax(f32 parallax,f32 dLevel,f32 addDist)
{

    nn::math::VEC4 *const pParallaxData = reinterpret_cast<nn::math::VEC4*>(&this->m_UniformProjBuffer[3 + 4 * 4]);
    pParallaxData->Set(parallax, dLevel, addDist, 0.0f);
}







}
}

