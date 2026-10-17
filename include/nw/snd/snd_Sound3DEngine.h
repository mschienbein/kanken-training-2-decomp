/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_Sound3DEngine.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SOUND_3D_ENGINE_H_
#define NW_SND_SOUND_3D_ENGINE_H_

#include "snd_Sound3DManager.h"
#include "snd_Sound3DCalculator.h"

namespace nw {
namespace snd {

class Sound3DEngine : public internal::ISound3DEngine
{
public:
    Sound3DEngine();
    virtual ~Sound3DEngine() {}
    virtual void UpdateAmbientParam(const Sound3DManager* sound3DManager, const Sound3DParam* sound3DParam, u32 soundId, u32 updateFlag, SoundAmbientParam* ambientParam);
    virtual int GetAmbientPriority(const Sound3DManager* sound3DManager, const Sound3DParam* sound3DParam, u32 soundId);
    virtual void UpdateAmbientParam(const Sound3DManager* sound3DManager, const Sound3DParam* sound3DParam, u32 soundId, SoundAmbientParam* ambientParam);
private:
    Sound3DCalculator::CalcPanParam m_CalcPanParam;
};

} // namespace snd
} // namespace nw

#endif // NW_SND_SOUND_3D_ENGINE_H_

