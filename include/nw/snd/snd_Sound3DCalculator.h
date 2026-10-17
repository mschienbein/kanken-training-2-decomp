/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_Sound3DCalculator.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SOUND_3D_CALCULATOR_H_
#define NW_SND_SOUND_3D_CALCULATOR_H_

#include "../../nn/math/math_Constant.h"
#include "../math/math_Types.h"
#include "snd_SoundArchive.h"

namespace nw {
namespace snd {

class Sound3DManager;
class Sound3DListener;
struct Sound3DParam;

class Sound3DCalculator
{
public:
    struct CalcPanParam
    {
        f32 stereoSpeakerAngle;
        f32 surroundSpeakerFrontAngle;
        f32 surroundSpeakerRearAngle;
        f32 surroundPanOffset;

        CalcPanParam(): 
            stereoSpeakerAngle(nn::math::PI / 4.0f),
            surroundSpeakerFrontAngle(nn::math::PI / 6.0f),
            surroundSpeakerRearAngle(nn::math::PI * 2.0f / 3.0f),
            surroundPanOffset(0.0f)
            {
        }
    };

    static void CalcVolumeAndPriority(const Sound3DManager& manager, const Sound3DListener& listener, const Sound3DParam& actorParam, f32* volumePtr, int* priorityPtr);
    static void CalcPan(const Sound3DManager& manager, const Sound3DListener& listener, const Sound3DParam& actorParam, const CalcPanParam& calcPanParam, f32* panPtr, f32* spanPtr);
    static void CalcPitch(const Sound3DManager& manager, const Sound3DListener& listener, const Sound3DParam& actorParam, f32* pitchPtr);
    static void CalcBiquadFilterValue(const Sound3DManager& manager, const Sound3DListener& listener, const Sound3DParam& actorParam, f32* biquadFilterValuePtr);
private:
    static void CalcVolumeAndPriorityImpl(f32 actorDistance, SoundArchive::Sound3DInfo::DecayCurve decayCurve, f32 decayRatio,
        int maxPriorityReduction, f32 maxVolumeDistance, f32 unitDistance, f32* volumePtr, int* priorityPtr);

    static void CalcPanImpl(const nw::math::VEC3& pos, f32 interiorSize, f32 actorDistance, f32 panRange,
        f32 stereoSpeakerAngle, f32 surroundSpeakerFrontAngle, f32 surroundSpeakerRearAngle, f32 surroundPanOffset,
        f32* panPtr, f32* surroundPanPtr);

    static void CalcPanSurround(const nw::math::VEC3& pos, f32 interiorSize, f32 actorDistance, f32 panRange, f32 surroundSpeakerFrontAngle, f32 surroundSpeakerRearAngle,
        f32 surroundPanOffset, f32* panPtr, f32* surroundPanPtr);
        
    static void CalcAngleAndDistance(
        const nw::math::VEC3& pos, f32 actorDistance,
        f32 interiorSize, f32* anglePtr, f32* distancePtr);
};

} // namespace snd
} // namespace nw

#endif // NW_SND_SOUND_3D_CALCULATOR_H_

