/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/anim/res/anim_ResUtil.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_ANIM_RESUTIL_H_
#define NW_ANIM_RESUTIL_H_

#include "anim_ResAnim.h"

namespace nw {
namespace anim {
namespace res {

ResAnim CloneTextureAnim(os::IAllocator* allocator, const ResAnim& src);

void DestroyClonedTextureAnim(os::IAllocator* allocator, ResAnim anim);

} // namespace res
} // namespace anim
} // namespace nw

#endif // NW_ANIM_RESUTIL_H_ 

