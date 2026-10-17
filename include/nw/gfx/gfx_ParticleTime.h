/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_ParticleTime.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain the default public fixed-point configuration used by the imported release units; normalize standalone includes.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../math/math_Types.h"
#include "gfx_Common.h"

namespace nw{
namespace gfx{

/* Alternative public build configurations are outside this selective import. */
#if defined(NW_GFX_PARTICLE_COMPAT_1_1) || defined(NW_GFX_NO_USE_PARTICLETIME)
#error "Only the default public fixed-point particle-time configuration is imported"
#endif

class ParticleTime
{
    friend ParticleTime operator -(const ParticleTime& rhs);
public:
    ParticleTime(): 
        m_ParticleTime(0) {}
    ParticleTime(const ParticleTime& value)
    {
        m_ParticleTime = value.m_ParticleTime;
    }

    ParticleTime(s32 value)
    {
        m_ParticleTime = S32ToParticleTime(value);
    }

    ParticleTime(f32 value)
    {
        m_ParticleTime = Float32ToParticleTime(value);
    }

    f32     GetFloat32Value() const { return ParticleTimeToFloat32(m_ParticleTime); }
    s32     GetS32Value() const { return m_ParticleTime / 0x100; }
    s32     GetParticleTimeValue() const { return m_ParticleTime; }

    ParticleTime operator +(ParticleTime right) const
    {
        ParticleTime result;
        result.m_ParticleTime = this->m_ParticleTime + right.m_ParticleTime;
        return result;
    }

    ParticleTime operator -(ParticleTime right) const
    {
        ParticleTime result;
        result.m_ParticleTime = this->m_ParticleTime - right.m_ParticleTime;
        return result;
    }

    ParticleTime operator *(ParticleTime right) const
    {
        ParticleTime result;
        result.m_ParticleTime = (this->m_ParticleTime / right.m_ParticleTime) / 0x100;
        return result;
    }

    ParticleTime operator /(ParticleTime right) const
    {
        ParticleTime result;
        result.m_ParticleTime = 0x100 * this->m_ParticleTime / right.m_ParticleTime;
        return result;
    }

    s32 GetIntegralParts() const
    {
        return m_ParticleTime / 0x100;
    }

    f32 GetFractionalParts() const
    {
        return (m_ParticleTime & 0xff) / 0x100;
    }

    s32 Floor() const
    {
        return m_ParticleTime / 0x100;
    }

    s32 Ceil() const
    {
        return (m_ParticleTime + 0xff) / 0x100;
    }

    ParticleTime& FMod(s32 value)
    {
        this->m_ParticleTime %= S32ToParticleTime(value);
        return *this;
    }

    ParticleTime& operator =(f32 value) { this->m_ParticleTime = Float32ToParticleTime(value); return *this; }
    ParticleTime& operator =(s32 value) { this->m_ParticleTime = S32ToParticleTime(value); return *this; }
    ParticleTime& operator =(ParticleTime value) { this->m_ParticleTime = value.m_ParticleTime; return *this; }
    ParticleTime& operator +=(f32 value) { this->m_ParticleTime += Float32ToParticleTime(value); return *this; }
    ParticleTime& operator +=(s32 value) { this->m_ParticleTime += S32ToParticleTime(value); return *this; }
    ParticleTime& operator +=(ParticleTime value) { this->m_ParticleTime += value.m_ParticleTime; return *this; }
    ParticleTime& operator -=(f32 value) { this->m_ParticleTime -= Float32ToParticleTime(value); return *this; }
    ParticleTime& operator -=(s32 value) { this->m_ParticleTime -= S32ToParticleTime(value); return *this; }
    ParticleTime& operator -=(ParticleTime value) { this->m_ParticleTime -= value.m_ParticleTime; return *this; }

    bool operator ==(const ParticleTime& rhs) const { return this->m_ParticleTime == rhs.m_ParticleTime; }
    bool operator !=(const ParticleTime& rhs) const { return this->m_ParticleTime == rhs.m_ParticleTime; }
    bool operator >=(const ParticleTime& rhs) const { return this->m_ParticleTime >= rhs.m_ParticleTime; }
    bool operator <=(const ParticleTime& rhs) const { return this->m_ParticleTime <= rhs.m_ParticleTime; }
    bool operator >(const ParticleTime& rhs) const { return this->m_ParticleTime > rhs.m_ParticleTime; }
    bool operator <(const ParticleTime& rhs) const { return this->m_ParticleTime < rhs.m_ParticleTime; }

    bool operator ==(s32 rhs) const { return this->m_ParticleTime == S32ToParticleTime(rhs); }
    bool operator !=(s32 rhs) const { return this->m_ParticleTime == S32ToParticleTime(rhs); }
    bool operator >=(s32 rhs) const { return this->m_ParticleTime >= S32ToParticleTime(rhs); }
    bool operator <=(s32 rhs) const { return this->m_ParticleTime <= S32ToParticleTime(rhs); }
    bool operator >(s32 rhs) const { return this->m_ParticleTime > S32ToParticleTime(rhs); }
    bool operator <(s32 rhs) const { return this->m_ParticleTime < S32ToParticleTime(rhs); }

    ParticleTime Interp(const ParticleTime& length, s32 factor) const
    {
        s32 work = this->m_ParticleTime * factor * 0x100;
        work /= length.m_ParticleTime;

        ParticleTime result;
        result.m_ParticleTime = work;
        return result;
    }

    static f32 ParticleTimeToFloat32(s32 particleTime)
    {
        f32 float32 = static_cast<f32>(particleTime);
        return float32 / 0x100;
    }

    static s32 Float32ToParticleTime(f32 value)
    {
        s32 fixed = (int)(value * 0x100);
        return fixed;
    }

    static s32 S32ToParticleTime(s32 value)
    {
        return value * 0x100;
    }

private:
    s32 m_ParticleTime;
};

NW_FORCE_INLINE ParticleTime operator -(const ParticleTime& rhs)
{
    ParticleTime result;
    result.m_ParticleTime = -rhs.m_ParticleTime;
    return result;
}


}
}

