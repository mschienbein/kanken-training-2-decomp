#include "nn/math/math_Vector4.h"

/* INTEGRATOR: move to include/game/matrix.h.
 * [INFERENCE] The only witnessed use initializes four consecutive floats to
 * (0, 0, 0, 1). VEC4 is the canonical four-float value type; quaternion versus
 * homogeneous-vector semantics and the original variable name are unrecovered. */
extern "C" {
extern nn::math::VEC4 bss_004b2aa4;
}

/* The compiler-generated initializer initializes this static-storage value. */
nn::math::VEC4 bss_004b2aa4(0.0f, 0.0f, 0.0f, 1.0f);
