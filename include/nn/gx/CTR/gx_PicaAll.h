/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned register/packing declarations. No Nintendo SDK header copied.
 * Register facts: libctru (zlib), https://github.com/devkitPro/libctru/blob/master/libctru/include/3ds/gpu/registers.h
 * Header/parameter bitfields: https://www.3dbrew.org/wiki/GPU/Internal_Registers
 * INDEXBUFFER_CONFIG is 0x227, NUMVERTICES 0x228. The index element width is bit 31.
 */
#pragma once
#include "gx_PicaPublic.h"
#include "gx_Registers.h"
#define PICA_REG_INDEX_ARRAY_ADDR_OFFSET 0x0227
#define PICA_REG_DRAW_VERTEX_NUM 0x0228
#define PICA_CMD_HEADER_SINGLE(reg) ((reg) | 0x000f0000U)
#define PICA_CMD_DATA_INDEX_ARRAY_ADDR_OFFSET(offset, is_u16) (((offset) & 0x7fffffffU) | ((unsigned)(is_u16) << 31))
#define PICA_CMD_DATA_DRAW_VERTEX_NUM(count) (count)
