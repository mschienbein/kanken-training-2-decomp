/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_IMaterialIdGenerator.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_GfxObject.h"
#include "../ut/ut_MoveArray.h"
#include "../ut/ut_RuntimeTypeInfo.h"

namespace nw{
namespace gfx{

class Material;

class IMaterialIdGenerator : public GfxObject
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(IMaterialIdGenerator);

public:
    NW_UT_RUNTIME_TYPEINFO;
    
    virtual void Accept(Material* material) = 0;
    virtual void Generate() = 0;

protected:
    IMaterialIdGenerator(nw::os::IAllocator* allocator) : 
        GfxObject(allocator)
    {
    }

};

}
}

