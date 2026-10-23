/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; adapted from sources/libraries/anim/res/anim_ResAnim.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected ResAnim cleanup and recovered target-version
 * texture-reference cache invalidation using the canonical resource types.
 */
#include <preinclude.h>
#include <nw/anim/res/anim_ResAnim.h>

namespace nw { namespace anim { namespace res {

void ResAnim::Cleanup()
{
    for (int i = 0; i < GetMemberAnimSetCount(); ++i) {
        ResMemberAnim member = GetMemberAnimSet(i);
        /* ORIGINAL BUG: serialized member and texture references are mandatory;
         * null references are not checked before accessing their data. */
        if (member.GetPrimitiveType() == ResMemberAnim::PRIMITIVETYPE_TEXTURE) {
            ResTextureAnim &textures = static_cast<ResTextureAnim &>(member);
            for (int j = 0; j < textures.GetTexturesCount(); ++j)
                textures.GetTextures(j).ref().toTargetTexture.set_ptr(NULL);
        }
    }
}

} } }
