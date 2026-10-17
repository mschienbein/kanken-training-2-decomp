/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_ItemType.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_ITEM_TYPE_H_
#define NW_SND_ITEM_TYPE_H_

namespace nw { 
namespace snd { 
namespace internal {

enum ItemType
{
    ItemType_Sound = 1,
    ItemType_SoundGroup,
    ItemType_Bank,
    ItemType_Player,
    ItemType_WaveArchive,
    ItemType_Group
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_ITEM_TYPE_H_

