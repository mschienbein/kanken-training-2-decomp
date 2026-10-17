/* SPDX-License-Identifier: GPL-3.0-or-later
 * Standalone release compilation adapter for pinned public NW4C; project-owned.
 * The clone's normal build supplies these macros and unqualified math aliases.
 * NON_MATCHING selects upstream's authored C++ alternatives, never assembly.
 */
#pragma once
#define NW_RELEASE
#define NW_PLATFORM_CTR
#define NN_PROCESSOR_ARM11MPCORE
#define NON_MATCHING
#define NN_UNUSED_VAR(variable) ((void)(variable))
#include "nw/types.h"
#include "nn/Assert.h"
#include "GLES2/gl2extern.h"
#include "nw/math/math_Types.h"
#include "nn/util/util_NonCopyable.h"
#include "nn/gx/CTR/gx_Transfer.h"
#include "nn/gx.h"
using namespace nw::math;
namespace nw { namespace ut {
typedef math::VEC3 ResVec3;
typedef math::MTX33 ResMtx33;
typedef math::MTX34 ResMtx34;
typedef math::MTX44 ResMtx44;
} }
