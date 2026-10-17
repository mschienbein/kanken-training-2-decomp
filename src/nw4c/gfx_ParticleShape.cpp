/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/gfx_ParticleShape.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: gfx_ParticleShape.cpp
//
// Project: NintendoWare4Ctr

#include <nn/gx.h>

#include <nw/gfx/gfx_ParticleShape.h>
#include <nw/gfx/gfx_ParticleUtil.h>
#include <nw/gfx/gfx_ParticleModel.h>
#include <nw/gfx/gfx_ParticleSet.h>
#include <nw/gfx/gfx_ParticleContext.h>
#include <nw/gfx/gfx_ParticleEmitter.h>
#include <nw/gfx/gfx_ISceneVisitor.h>
#include <nw/gfx/res/gfx_ResVertex.h>

#include <nw/ut/ut_ResUtil.h>
#include <nw/ut/ut_ResDictionary.h>
#include <nw/ut/ut_Foreach.h>

namespace nw{
namespace gfx{

static const int MAX_ATTRIBUTES_NUM = 12;





static const int PrimitiveCommandSize = 26;




/* ParticleShape */





















void ParticleShape::FlushBuffer()
{
    for (s32 i = 0; i < this->GetVertexAttributesCount(); ++i)
    {
        if (this->IsVertexStream(i))
        {
            nngxUpdateBuffer(this->GetVertexStreamPtr(i, PARTICLE_BUFFER_FRONT), this->GetVertexCapacity() * this->GetVertexAttributeDimension(i) * sizeof(f32));
        }
    }

    nngxUpdateBuffer(this->GetPrimitiveStreamPtr(PARTICLE_BUFFER_FRONT), this->GetVertexCapacity() * sizeof(u16));
}

}
}

