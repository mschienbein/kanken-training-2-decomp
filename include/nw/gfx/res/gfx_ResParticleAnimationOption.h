/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/res/gfx_ResParticleAnimationOption.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../../ut/ut_ResUtil.h"
#include "../../ut/ut_ResDictionary.h"
#include "../../ut/ut_ResDeclMacros.h"
#include "gfx_ResTypeInfo.h"

namespace nw {
namespace gfx {
namespace res {

struct ResParticleAnimationOptionData
{
    nw::ut::ResTypeInfo typeInfo;
    nw::ut::ResF32 m_AnimationDuration;
};

class ResParticleAnimationOption : public nw::ut::ResCommon< ResParticleAnimationOptionData >
{
public:
    enum
{ TYPE_INFO = NW_GFX_RES_TYPE_INFO(ResParticleAnimationOption) };
    enum
{ SIGNATURE = NW_RES_SIGNATURE32('AOPT') };

    NW_RES_CTOR( ResParticleAnimationOption )


    f32 GetAnimationDuration() const { return static_cast<f32>(ref().m_AnimationDuration); }
    void SetAnimationDuration(f32 value) { ref().m_AnimationDuration = value; }

    nw::ut::ResTypeInfo     GetTypeInfo() const { return ref().typeInfo; }

    static u32 GetHash(u32 key1, u32 key2, u32 key3, u32 key4);
//    ParticleTime EvaluateAnimationFrame(u32 id, ParticleTime birth, ParticleTime life, ParticleTime time, int updaterIndex) const;
};

struct ResParticleFittingAnimationOptionData : public ResParticleAnimationOptionData
{};

class ResParticleFittingAnimationOption : public ResParticleAnimationOption
{
public:
    enum
{ TYPE_INFO = NW_GFX_RES_TYPE_INFO(ResParticleFittingAnimationOption) };
    enum
{ SIGNATURE = NW_RES_SIGNATURE32('FITT') };

    NW_RES_CTOR_INHERIT( ResParticleFittingAnimationOption, ResParticleAnimationOption )
};

struct ResParticleFrameLoopAnimationOptionData : public ResParticleAnimationOptionData
{
    nw::ut::ResU32 m_LoopDuration;
    nw::ut::ResBool m_RandomOffset;
    u8              padding_0[3];
};

class ResParticleFrameLoopAnimationOption : public ResParticleAnimationOption
{
public:
    enum
{ TYPE_INFO = NW_GFX_RES_TYPE_INFO(ResParticleFrameLoopAnimationOption) };
    enum
{ SIGNATURE = NW_RES_SIGNATURE32('FLOO') };

    NW_RES_CTOR_INHERIT( ResParticleFrameLoopAnimationOption, ResParticleAnimationOption )


    u32 GetLoopDuration() const { return static_cast<u32>(ref().m_LoopDuration); }
    void SetLoopDuration(u32 value) { ref().m_LoopDuration = value; }

    bool GetRandomOffset() const { return static_cast<bool>(ref().m_RandomOffset); }
    void SetRandomOffset(bool value) { ref().m_RandomOffset = value; }
};

struct ResParticleRandomAnimationOptionData : public ResParticleAnimationOptionData
{};

class ResParticleRandomAnimationOption : public ResParticleAnimationOption
{
public:
    enum
{ TYPE_INFO = NW_GFX_RES_TYPE_INFO(ResParticleRandomAnimationOption) };
    enum
{ SIGNATURE = NW_RES_SIGNATURE32('RAND') };

    NW_RES_CTOR_INHERIT( ResParticleRandomAnimationOption, ResParticleAnimationOption )
};

}
}
}

