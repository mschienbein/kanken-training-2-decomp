#include <preinclude.h>
#include "game/material.h"
#include "nw/gfx/gfx_CommandUtil.h"
#include "nn/gx/CTR/gx_PicaAll.h"

using nw::gfx::internal::MakeCommandHeader;

/* Twelve attributes, all fixed, and no high attribute-format bits. The
 * fixed-attribute entries select slots 1 through 11 with zero vectors.
 * The native helper dynamically initializes only the command headers.
 * [INFERENCE] The original variable spelling and translation unit are unknown. */
const unsigned data_00243148[0x5c] = {
    ((12u - 1u) << 28) | (((1u << 12) - 1u) << 16),
    MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 2, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 5, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 8, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 11, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 14, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 17, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 20, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 23, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 26, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 29, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 32, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 35, 1, false, 15),
    0, MakeCommandHeader(PICA_REG_VTX_ATTR_ARRAYS_BASE_ADDR + 38, 1, false, 15),
    1, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    2, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    3, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    4, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    5, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    6, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    7, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    8, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    9, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    10, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0,
    11, MakeCommandHeader(PICA_REG_VS_FIXED_ATTR, 4, true, 15), 0, 0, 0, 0
};
