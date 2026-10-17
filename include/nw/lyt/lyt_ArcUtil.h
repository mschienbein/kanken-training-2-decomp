/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_ArcUtil.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "lyt_Arc.h"
#include "lyt_ResourceAccessor.h"
#include "lyt_Resources.h"
#include "lyt_Types.h"

namespace nw {
namespace lyt {

class ArcUtil
{
public:
    static bool OpenTextureDir(ARCHandle* pArcHandle, const wchar_t* pRootName, ARCDir* pDir)
    {
        return OpenResourceDir(pArcHandle, pRootName, res::RESOURCETYPE_TEXTURE, pDir);
    }

    static bool ReadTextureDir(ARCDir* pDir, ARCDirEntry* pDirEnt)
    {
        return ReadResourceDir(pDir, L".bclim", 6, pDirEnt);
    }

    static bool OpenFontDir(ARCHandle* pArcHandle, const wchar_t* pRootName, ARCDir* pDir)
    {
        return OpenResourceDir(pArcHandle, pRootName, res::RESOURCETYPE_FONT, pDir);
    }

    static bool ReadFontDir(ARCDir* pDir, ARCDirEntry* pDirEnt)
    {
        return ReadResourceDir(pDir, L".bcfnt", 6, pDirEnt);
    }

    static bool OpenResourceDir(ARCHandle* pArcHandle, const wchar_t* pRootName, ResType resType, ARCDir* pDir);
    static bool ReadResourceDir(ARCDir* pDir, const wchar_t* pExtStr, u32 extLen, ARCDirEntry* pDirEnt);
};

}
}

