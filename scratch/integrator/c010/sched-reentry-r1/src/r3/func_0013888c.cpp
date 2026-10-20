/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; adapted from the preserved public source at
 * ctrdecomp/NintendoWare4Ctr commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified 2026-10-20: shared native-value lookup callback contract and complete
 * original dispatch-provider ownership.
 */
#include "game/graphics_resource_abi.h"
#include <nw/gfx/gfx_GraphicsDevice.h>

/* The shared dispatcher's image tag establishes the payload projection. */
void func_001c6a34(nw::gfx::res::ResLookupTable object)
{
    nw::gfx::GraphicsDevice::InvalidateLookupTable(
        nw::gfx::res::ResImageLookupTable(object.ptr()));
}

/* The reference tag selects the actual serialized target offset to clear. */
void func_001c7f10(nw::gfx::res::ResLookupTable object)
{
    nw::gfx::res::ResReferenceLookupTable(object.ptr()).ref().toTargetLut = 0;
}

namespace nw { namespace gfx { namespace res {

void ResLookupTable::Cleanup()
{
    ResLookupTableData *resource = ptr();
    if (resource->typeInfo == ResImageLookupTable::TYPE_INFO)
        data_002434e4[0](ResLookupTable(resource));
    else if (resource->typeInfo == ResReferenceLookupTable::TYPE_INFO)
        data_002434e4[1](ResLookupTable(resource));
}

void ResLookupTableSet::Cleanup()
{
    /* ORIGINAL BUG: a missing sampler dictionary with a nonzero count still
     * constructs invalid iterator bounds and is traversed without validation. */
    ut::SafeCleanupAll(this->GetSamplers());
}

} } }
