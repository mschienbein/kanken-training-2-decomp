/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/ut/ut_ResTypeInfo.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../types.h"

namespace nw {
namespace ut {
namespace internal {

#define NW_UT_RES_TYPE_INFO(name)  internal::name##_TYPE_INFO

enum ResTypeInfoDefinition
{
    ResMetaData_TYPE_INFO               = 0x00000000,
    ResFloatArrayMetaData_TYPE_INFO     = 0x80000000,
    ResColorArrayMetaData_TYPE_INFO     = 0x40000000,
    ResIntArrayMetaData_TYPE_INFO       = 0x20000000,
    ResStringArrayMetaData_TYPE_INFO    = 0x10000000,
    ResVector3ArrayMetaData_TYPE_INFO   = 0x08000000,
    ResBoundingVolume_TYPE_INFO         = 0x00000000,
    ResOrientedBoundingBox_TYPE_INFO    = 0x80000000,
    ResAxisAlignedBoundingBox_TYPE_INFO = 0x40000000
};

}
}
}

