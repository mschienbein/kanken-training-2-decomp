/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_BiquadFilterCallback.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_BIQUAD_FILTER_CALLBACK_H_
#define NW_SND_BIQUAD_FILTER_CALLBACK_H_

#include "../../nn/types.h"
#include "../../nn/snd.h"

namespace nw {
namespace snd {

class BiquadFilterCallback
{
public:
    typedef nn::snd::CTR::BiquadFilterCoefficients Coefficients;

    virtual ~BiquadFilterCallback() {}
    virtual void GetCoefficients( int type, f32 value, Coefficients* coef ) const = 0;
};

} // namespace nw
} // namespace snd

#endif // NW_SND_BIQUAD_FILTER_CALLBACK_H_

