/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_CurveLfo.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_CURVE_LFO_H_
#define NW_SND_CURVE_LFO_H_

#include "../../nn/types.h"

namespace nw {
namespace snd {
namespace internal {

struct CurveLfoParam
{
    f32 depth;
    f32 speed;
    u32 delay;
    u8  range;
    u8  padding[3];

    CurveLfoParam() { Initialize(); }
    void Initialize();
};

class CurveLfo
{
private:
    static const int TABLE_SIZE = 32;
    static const int PERIOD = TABLE_SIZE * 4;

private:
    static s8 GetSinIdx(int index);

public:
    CurveLfo() : 
        m_DelayCounter(0), 
        m_Counter(0.0f) 
        {
    }
    void Reset();

    void Update(int msec);
    f32  GetValue() const;

    void SetParam(const CurveLfoParam& lfoParam) { m_Param = lfoParam; }
    CurveLfoParam& GetParam() { return m_Param; }
    const CurveLfoParam& GetParam() const { return m_Param; }

private:
    CurveLfoParam m_Param;
    u32      m_DelayCounter;
    f32      m_Counter;
};

} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_CURVE_LFO_H_

