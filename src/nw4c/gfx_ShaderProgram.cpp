/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/gfx_ShaderProgram.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: gfx_ShaderProgram.cpp
//
// Project: NintendoWare4Ctr

#include <nw/gfx/gfx_ShaderProgram.h>

#include <nw/os/os_Memory.h>
#include <nw/ut/ut_Foreach.h>
#include <nw/gfx/gfx_ShaderBinaryInfo.h>
#include <nw/gfx/gfx_ActivateCommand.h>

namespace nw{
namespace gfx{

ShaderProgram::ShaderProgram(os::IAllocator* allocator): 
    GfxObject(allocator),
    m_ProgramObject(0),
    m_GeometryShaderMode(GEOMETRY_SHADER_MODE_UNINITIALIZED)
    {
    const u32 REG_VERTEX_UNIFORM_BOOL = 0x2B0;
    const u32 REG_GEOMETRY_UNIFORM_BOOL = 0x280;
    const u32 REG_GEOMETRY_DISABLE = 0x244;
    
    const u32 HEADER_VERTEX   = internal::MakeCommandHeader(REG_VERTEX_UNIFORM_BOOL, 5, true, 0xF);
    const u32 HEADER_GEOMETRY = internal::MakeCommandHeader(REG_GEOMETRY_UNIFORM_BOOL, 5, true, 0xF);
    const u32 HEADER_DISABLE_GEOMETRY = internal::MakeCommandHeader(REG_GEOMETRY_DISABLE, 1, false, 0x1);
    
    m_VertexIntUniforms[0] = 0x7fff0000; // b0-b15
    m_VertexIntUniforms[1] = HEADER_VERTEX,
    m_VertexIntUniforms[2] = 0x00000000; // i0
    m_VertexIntUniforms[3] = 0x00000000; // i1
    m_VertexIntUniforms[4] = 0x00000000; // i2
    m_VertexIntUniforms[5] = 0x00000000; // i3

    m_GeometryIntUniforms[0] = 0x7fff0000; // b0-b15
    m_GeometryIntUniforms[1] = HEADER_GEOMETRY,
    m_GeometryIntUniforms[2] = 0x00000000; // i0
    m_GeometryIntUniforms[3] = 0x00000000; // i1
    m_GeometryIntUniforms[4] = 0x00000000; // i2
    m_GeometryIntUniforms[5] = 0x00000000; // i3
    
    m_DisableGeometry[0] = 0;
    m_DisableGeometry[1] = HEADER_DISABLE_GEOMETRY;
}







}
}

