/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/res/gfx_ResUtil.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <utility>

namespace nw {
namespace gfx {

class Result;

namespace res {

class ResLookupTable;
class ResTexture;
class ResShader;
class ResReferenceLookupTable;
class ResReferenceTexture;
class ResReferenceShader;
class ResGraphicsFile;

Result SetupReferenceLut(ResReferenceLookupTable resReferenceLut, ResGraphicsFile graphicsFile);

::std::pair<ResLookupTable, bool>
    GetReferenceLutTarget(ResReferenceLookupTable referenceLut, ResGraphicsFile graphicsFile);

::std::pair<ResTexture,bool>
    GetReferenceTextureTarget(ResReferenceTexture referenceTexture, ResGraphicsFile graphicsFile);

::std::pair<ResShader,bool>
    GetReferenceShaderTarget(ResReferenceShader referenceShader, ResGraphicsFile graphicsFile);

}
}
}

