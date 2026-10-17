/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_MaterialActivator.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_GfxObject.h"
#include "gfx_IMaterialActivator.h"

namespace nw{
namespace os{
    class IAllocator;
}
namespace gfx{

class RenderContext;
class Material;

class MaterialActivator : public IMaterialActivator
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(MaterialActivator);

public:
    NW_UT_RUNTIME_TYPEINFO;

    static MaterialActivator* Create(nw::os::IAllocator* allocator);

    static size_t GetMemorySize(size_t alignment = nw::os::IAllocator::DEFAULT_ALIGNMENT)
    {
        nw::os::MemorySizeCalculator size(alignment);

        GetMemorySizeInternal(&size);

        return size.GetSizeWithPadding(alignment);
    }

    static void GetMemorySizeInternal(nw::os::MemorySizeCalculator* pSize)
    {
        nw::os::MemorySizeCalculator& size = *pSize;
        
        size += sizeof(MaterialActivator);
    }

    virtual void Activate(RenderContext* renderContext, const Material* material);

private:
    MaterialActivator(nw::os::IAllocator* allocator);
    virtual ~MaterialActivator();
};

}
}

