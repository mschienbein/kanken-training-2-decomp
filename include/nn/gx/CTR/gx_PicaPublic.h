/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned PICA facts and register packing, not Nintendo SDK source.
 * Numeric enums/formats: https://github.com/devkitPro/libctru/blob/master/libctru/include/3ds/gpu/enums.h (zlib)
 * Blend packing: https://github.com/devkitPro/citro3d/blob/master/source/effect.c (zlib)
 * Registers, command headers, uniform type and output semantics:
 * https://www.3dbrew.org/wiki/GPU/Internal_Registers
 * SDK spellings map the public hardware semantics used in pinned NW4C. The
 * aliases/packing remain [INFERENCE] until a complete candidate MATCH receipt.
 */
#pragma once
#define PICA_REG_COLOR_DEPTH_BUFFER_CLEAR0 0x0110
#define PICA_REG_COLOR_DEPTH_BUFFER_CLEAR1 0x0111
#define PICA_REG_VERTEX_ATTR_ARRAYS_BASE_ADDR 0x0200
#define PICA_REG_TEX_ENV3 0x00d8
#define PICA_REG_TEX_ENV5 0x00f8
#define PICA_REG_VS_OUT_REG_NUM0 0x004f
#define PICA_REG_VS_OUT_ATTR0 0x0050
#define PICA_REG_VS_OUT_ATTR_MODE 0x0064
#define PICA_REG_VS_OUT_ATTR_CLK 0x006f
#define PICA_REG_VS_FLOAT0 0x02c1
#define PICA_DATA_DRAW_TRIANGLES 0
#define PICA_DATA_FRAGOP_MODE_DMP 0
#define PICA_DATA_ENABLE_BLEND 1
#define PICA_DATA_BLEND_EQUATION_ADD 0
#define PICA_DATA_BLEND_FUNC_SRC_ALPHA 6
#define PICA_DATA_BLEND_FUNC_ONE_MINUS_SRC_ALPHA 7
#define PICA_DATA_FOG_FALSE 0
#define PICA_DATA_SIZE_4_FLOAT 15
#define PICA_DATA_SIZE_1_BYTE 0
#define PICA_DATA_VS_F24 0
#define PICA_DATA_VS_OUT_ATTR_X 0
#define PICA_DATA_VS_OUT_ATTR_QUART_X 4
#define PICA_DATA_VS_OUT_ATTR_R 8
#define PICA_DATA_VS_OUT_ATTR_TEX0_U 12
#define PICA_DATA_VS_OUT_ATTR_TEX0_W 16
#define PICA_DATA_VS_OUT_ATTR_TEX1_U 14
#define PICA_DATA_TEX_ENV_SRC_RGBA_PRIMARY_COLOR 0
#define PICA_DATA_VS_OUT_ATTR_TEX2_U 22
#define PICA_DATA_VS_OUT_ATTR_VIEW_X 18
#define PICA_DATA_VS_OUT_ATTR_INVALID 31
#define PICA_DATA_TEX_ENV_SRC_RGBA_TEXTURE0 3
#define PICA_DATA_TEX_ENV_SRC_RGBA_CONSTANT 14
#define PICA_DATA_OPE_RGB_SRC_COLOR 0
#define PICA_DATA_OPE_RGB_ONE_MINUS_SRC_COLOR 1
#define PICA_DATA_OPE_ALPHA_SRC_ALPHA 0
#define PICA_DATA_OPE_ALPHA_ONE_MINUS_SRC_ALPHA 1
#define PICA_DATA_TEX_ENV_COMBINE_MODULATE 1
#define PICA_DATA_TEX_ENV_SCALE_1 0
#define PICA_CMD_DATA_COLOR_OPERATION(mode, blend) ((mode) | ((blend) << 8) | 0x00e40000U)
#define PICA_CMD_DATA_BLEND_FUNC(eq, src, dst) ((eq) | ((eq) << 8) | ((src) << 16) | ((dst) << 20) | ((src) << 24) | ((dst) << 28))
#define PICA_CMD_DATA_FRAGOP_ALPHA_TEST_DISABLE() 0
#define PICA_CMD_HEADER_BURSTSEQ(reg, count) ((reg) | 0x800f0000U | (((count)-1) << 20))
#define PICA_CMD_DATA_TEX_ENV_SRC(r0, r1, r2, a0, a1, a2) ((r0) | ((r1) << 4) | ((r2) << 8) | ((a0) << 16) | ((a1) << 20) | ((a2) << 24))
#define PICA_CMD_DATA_TEX_ENV_OPERAND(r0, r1, r2, a0, a1, a2) ((r0) | ((r1) << 4) | ((r2) << 8) | ((a0) << 12) | ((a1) << 16) | ((a2) << 20))
#define PICA_CMD_DATA_TEX_ENV_COMBINE(rgb, alpha) ((rgb) | ((alpha) << 16))
#define PICA_CMD_DATA_TEX_ENV_BUFFER_COLOR_WHITE 0xffffffffU
#define PICA_CMD_DATA_TEX_ENV_SCALE(rgb, alpha) ((rgb) | ((alpha) << 16))
