/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_SoundMemoryAllocatable.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SOUND_MEMORY_ALLOCATABLE_H_
#define NW_SND_SOUND_MEMORY_ALLOCATABLE_H_

#include "snd_Global.h"

namespace nw { 
namespace snd {

class SoundMemoryAllocatable
{
public:
    virtual ~SoundMemoryAllocatable() {}

    virtual void* Alloc(size_t size) = 0;
};

} // namespace snd
} // namespace nw

#endif // NW_SND_SOUND_MEMORY_ALLOCATABLE_H_

