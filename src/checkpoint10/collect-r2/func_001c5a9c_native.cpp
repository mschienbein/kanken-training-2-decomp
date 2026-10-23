/* SPDX-License-Identifier: GPL-3.0-only
 * Uses the pinned public NintendoWare4Ctr CharStrmReader layout and helpers.
 * Method identity is [INFERENCE] from the Shift-JIS behavior and callback shape.
 */
#define NW_RELEASE
#include <nw/font/font_CharStrmReader.h>

namespace nw { namespace font {
u16 CharStrmReader::ReadNextCharSJIS()
{
    u16 character = GetChar<u8>();
    if (static_cast<unsigned int>(character - 0x81) < 0x1f || character >= 0xe0) {
        character = (character << 8) | GetChar<u8>(1);
        StepStrm<u8>(2);
    } else {
        StepStrm<u8>();
    }
    return character;
}
} }
