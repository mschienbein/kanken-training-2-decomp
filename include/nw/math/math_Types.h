/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/math/math_Types.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../../nn/math/math_Arithmetic.h"
#include "../../nn/math/math_Utility.h"
#include "../../nn/math/math_Matrix23.h"
#include "../../nn/math/math_Matrix33.h"
#include "../../nn/math/math_Matrix34.h"
#include "../../nn/math/math_Matrix44.h"
#include "../../nn/math/math_Vector2.h"
#include "../../nn/math/math_Vector3.h"
#include "../../nn/math/math_Vector4.h"
#include "../../nn/math/math_Quaternion.h"
#include "../../nn/math/math_Transform.h"
#include "../../nn/math/math_Triangular.h"

namespace nw { 
namespace math {
using nn::math::MTX23;
using nn::math::MTX33;
using nn::math::MTX34;
using nn::math::MTX44;
using nn::math::VEC2;
using nn::math::VEC3;
using nn::math::VEC4;
using nn::math::QUAT;
using nn::math::Transform2;
using nn::math::Transform3;
using namespace nn::math;

}
}

