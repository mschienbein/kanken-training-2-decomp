/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_FxBase.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_FX_BASE_H_
#define NW_SND_FX_BASE_H_

#include "../../nn/snd.h"
#include "../ut/ut_LinkList.h"
#include "../ut/ut_Preprocessor.h"
#include "snd_Global.h"

namespace nw{
namespace snd{

class FxBase
{
public:
    virtual ~FxBase() {}
    virtual bool Initialize() = 0;
    virtual void Finalize() = 0;
    virtual void UpdateBuffer(int numChannels,nn::snd::CTR::AuxBusData* data,s32 sampleLength,SampleFormat format,f32 sampleRate,OutputMode mode) = 0;
    virtual void OnChangeOutputMode() {}

    ut::LinkListNode m_Link;
};

} // namespace nw
} // namespace snd

#endif // NW_SND_FX_BASE_H_

