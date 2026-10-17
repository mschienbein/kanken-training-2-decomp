/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/gfx_GraphicsDevice.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: gfx_GraphicsDevice.cpp
//
// Project: NintendoWare4Ctr

#include <nw/gfx/gfx_GraphicsDevice.h>
#include <nw/gfx/gfx_CommandUtil.h>
#include <nw/gfx/res/gfx_ResMaterial.h>
#include <nw/gfx/res/gfx_ResLookupTable.h>

namespace nw{
namespace gfx{

enum
{ 
    REG_BUFFER_CACHE_CLEAR     = 0x110,
    REG_BUFFER_CACHE_TAG_CLEAR = 0x111,
    REG_BUFFER_MASK_BASE       = 0x112
};

































void GraphicsDevice::ActivateLookupTable(res::ResImageLookupTable lookupTable, LutTarget target)
{
    NW_ASSERT(lookupTable.IsValid());

    if (m_LutTargets[target] != lookupTable)
    {
        m_LutTargets[target] =  lookupTable;
        ActivateLutLoadSetting(target);
        internal::NWUseCmdlist( lookupTable.GetCommandCache(), lookupTable.GetCommandCacheCount() );
    }
}

void GraphicsDevice::InvalidateLookupTable(res::ResImageLookupTable lookupTable)
{
    NW_ASSERT(lookupTable.IsValid());

    for (int target = 0; target < LUT_TARGET_COUNT; ++target)
    {
        if (m_LutTargets[target] == lookupTable)
        {
            m_LutTargets[target] = res::ResImageLookupTable(NULL);
        }
    }
}

void GraphicsDevice::InvalidateAllLookupTables()
{
    for (int target = 0; target < LUT_TARGET_COUNT; ++target)
    {
        m_LutTargets[target] = res::ResImageLookupTable(NULL);
    }
}



}
}

