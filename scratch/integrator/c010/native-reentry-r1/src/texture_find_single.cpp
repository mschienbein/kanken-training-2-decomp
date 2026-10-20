/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; sources/libraries/lyt/lyt_TextureContainer.cpp
 * Pinned upstream: 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified 2026-10-20: select native lookup; use one returned value.
 * GNU GPL version 3; see src/nw4c/LICENSE.txt.
 */
#include <preinclude.h>
#include <cstring>
#include <nw/lyt/lyt_TextureContainer.h>

namespace nw { namespace lyt {
const TextureInfo TextureContainer::FindTextureByName(const char* name)
{
    TextureInfo result;
    for (Iterator it = GetBeginIter(); it != GetEndIter(); ++it) {
        if (std::strcmp(name, it->GetResourceName()) == 0) {
            result = it->GetTextureInfo();
            break;
        }
    }
    return result;
}
} }
