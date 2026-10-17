/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_BiquadFilterPresets.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: relative includes; declare implicit virtual destructors out of line so standalone coefficient methods leave original VFE-stripped vtables in the image.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_BIQUAD_FILTER_PRESETS_H_
#define NW_SND_BIQUAD_FILTER_PRESETS_H_

#include "snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {

class BiquadFilterLpf : public BiquadFilterCallback
{
public:
    BiquadFilterLpf() {}
    virtual ~BiquadFilterLpf();
    virtual void GetCoefficients(int type, f32 value, Coefficients* coef) const;

private:
    static const int COEFFICIENTS_TABLE_SIZE = 112;
    static const Coefficients COEFFICIENTS_TABLE[COEFFICIENTS_TABLE_SIZE];
};

class BiquadFilterHpf : public BiquadFilterCallback
{
public:
    BiquadFilterHpf() {}
    virtual ~BiquadFilterHpf();
    virtual void GetCoefficients(int type, f32 value, Coefficients* coef) const;

private:
    static const int COEFFICIENTS_TABLE_SIZE = 97;
    static const Coefficients COEFFICIENTS_TABLE[COEFFICIENTS_TABLE_SIZE];
};

class BiquadFilterBpf512 : public BiquadFilterCallback
{
public:
    BiquadFilterBpf512() {}
    virtual ~BiquadFilterBpf512();
    virtual void GetCoefficients(int type, f32 value, Coefficients* coef) const;

private:
    static const int COEFFICIENTS_TABLE_SIZE = 122;
    static const Coefficients COEFFICIENTS_TABLE[COEFFICIENTS_TABLE_SIZE];
};

class BiquadFilterBpf1024 : public BiquadFilterCallback
{
public:
    BiquadFilterBpf1024() {}
    virtual ~BiquadFilterBpf1024();
    virtual void GetCoefficients(int type, f32 value, Coefficients* coef) const;

private:
    static const int COEFFICIENTS_TABLE_SIZE = 93;
    static const Coefficients COEFFICIENTS_TABLE[COEFFICIENTS_TABLE_SIZE];
};

class BiquadFilterBpf2048 : public BiquadFilterCallback
{
public:
    BiquadFilterBpf2048() {}
    virtual ~BiquadFilterBpf2048();
    virtual void GetCoefficients(int type, f32 value, Coefficients* coef) const;

private:
    static const int COEFFICIENTS_TABLE_SIZE = 93;
    static const Coefficients COEFFICIENTS_TABLE[COEFFICIENTS_TABLE_SIZE];
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_BIQUAD_FILTER_PRESETS_H_

