#ifndef GAME_BIQUAD_FILTER_H
#define GAME_BIQUAD_FILTER_H

struct BiquadCoefficients;
#ifdef __cplusplus
class BiquadFilterCallback;
#endif



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */

struct BiquadCoefficients { short values[5]; };

extern "C" {

extern const BiquadCoefficients data_0021d7ce[];
extern const BiquadCoefficients data_0021db70[];
}
#endif

#endif
