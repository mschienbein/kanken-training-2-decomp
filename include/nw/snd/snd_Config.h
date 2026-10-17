/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_Config.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_CONFIG_H_
#define NW_SND_CONFIG_H_

#include "../types.h"
#include <cstring>

namespace nw { 
namespace snd { 
namespace internal {

static const u32 STRM_TRACK_NUM = 8;

} // namespace nw
} // namespace snd
} // namespace internal

#endif // NW_SND_CONFIG_H_

