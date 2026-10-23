/* SPDX-License-Identifier: GPL-3.0-or-later
 * Independently authored from uses in public NintendoWare4Ctr include/nw/snd/
 * and sources/libraries/snd/, commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * No Nintendo SDK header consulted or copied. Channel/bus ordinal values are
 * inferred from NW4C's zero-based bus and four-channel array indexing; complete
 * candidate matching is required before any declaration is credited as ABI proof.
 */
#pragma once
#include "types.h"
/* Frame length attested by libctru's ndspSync capture count (zlib license),
 * commit 9b55eda44cf80b971503991e0c78f9bc8fe50425, source/ndsp/ndsp.c.
 * Project-owned SDK spelling; no libctru implementation or SDK code copied.
 * See src/nw4c/FACTS-HEADER-GAPS.json and LICENSE-LIBCTRU.txt.
 */
#define NN_SND_SAMPLES_PER_FRAME 160
namespace nn { namespace snd { namespace CTR {
enum AuxBusId { AUX_BUS_A = 0, AUX_BUS_B = 1, AUX_BUS_NUM = 2, AUX_BUS_NULL = -1 };
enum ChannelIndex { CHANNEL_INDEX_FRONT_LEFT, CHANNEL_INDEX_FRONT_RIGHT,
    CHANNEL_INDEX_REAR_LEFT, CHANNEL_INDEX_REAR_RIGHT, CHANNEL_INDEX_NUM };
struct MixParam { f32 mainBus[CHANNEL_INDEX_NUM]; f32 auxBusA[CHANNEL_INDEX_NUM]; f32 auxBusB[CHANNEL_INDEX_NUM]; };
struct BiquadFilterCoefficients { s16 b0, b1, b2, a1, a2; };
struct AuxBusData { s32* frontLeft; s32* frontRight; s32* rearLeft; s32* rearRight; };
struct AdpcmParam { s16 coef[16]; };
struct AdpcmContext { u16 pred_scale; s16 yn1; s16 yn2; };
struct WaveBuffer;
class Voice;
class FxDelay;
class FxReverb;
enum { VOICE_PRIORITY_NODROP = 255 };
void FlushDataCache(uptr address, size_t size);
void InvalidateDataCache(uptr address, size_t size);
} } }
