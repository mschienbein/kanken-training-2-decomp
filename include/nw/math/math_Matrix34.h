/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/math/math_Matrix34.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "math_Types.h"

namespace nw { 
namespace math {

VEC3*  MTX34DecomposeToColumnScale(VEC3* pOut, const MTX34* pM);
MTX34* MTX34CameraRotateRad(MTX34* pOut, const VEC3* pCamPos, const VEC3* pCamRotate);

}
}

