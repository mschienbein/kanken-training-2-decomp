/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_ParticleRandom.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../types.h"

namespace nw{
namespace gfx{

class ParticleRandom
{
public:
    ParticleRandom()
    {
        m_Seed = 0;
    }

    ParticleRandom(const ParticleRandom& source)
    {
        Set(source);
    }

    void Srand(u32 seed)
    {
        m_Seed = seed;
    }

    u16 Next(u16 maxValue)
    {
        MixRandomSeed();
        return (u16)(m_Seed >> 8) % maxValue;
    }

    f32 NextFloat()
    {
        MixRandomSeed();
        return (f32)((m_Seed >> 16) & 0xffff) / 65536.0f;
    }

    f32 NextFloatSignedOne();

    f32 NextFloatSignedHalf();

    void Set(const ParticleRandom& source)
    {
        m_Seed = source.m_Seed;
    }

private:
    void MixRandomSeed()
    {
        m_Seed = (m_Seed * 214013u) + 2531011u;
    }

    u32 m_Seed;
};

}
}

