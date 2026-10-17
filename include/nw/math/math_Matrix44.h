/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/math/math_Matrix44.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "math_Types.h"

namespace nw { 
namespace math {

MTX44* MTX44TextureMatrixForMax(MTX44* pOut,float scaleS, float scaleT,float rotate,float translateS, float translateT);
MTX44* MTX44TextureMatrixForMaya(MTX44* pOut,float scaleS, float scaleT,float rotate,float translateS, float translateT);
MTX44* MTX44TextureMatrixForSoftimage(MTX44* pOut,float scaleS, float scaleT,float rotate,float translateS, float translateT);

}
}

