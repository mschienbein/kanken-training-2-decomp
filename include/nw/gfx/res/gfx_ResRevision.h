/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/res/gfx_ResRevision.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../../types.h"

namespace nw {
namespace gfx {
namespace res {

enum GraphicsBinaryRevisions
{
    REVISION_RES_GRAPHICS_FILE = 0x05000000,
    REVISION_RES_SHADER        = 0x06000000,
    REVISION_RES_LUT_SET       = 0x04000000,
    REVISION_RES_TEXTURE       = 0x05000000,
    REVISION_RES_MATERIAL      = 0x06000000,
    REVISION_RES_MODEL         = 0x09000000,
    REVISION_RES_CAMERA        = 0x07010000,
    REVISION_RES_LIGHT         = 0x07010000,
    REVISION_RES_FOG           = 0x06000000,
    REVISION_RES_EMITTER       = 0x05000000,
    REVISION_RES_RENDER_ENV_SETTING = 0x01000000
};

}
}
}

