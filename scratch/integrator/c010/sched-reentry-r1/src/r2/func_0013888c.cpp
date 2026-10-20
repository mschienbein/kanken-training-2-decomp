/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; adapted from the preserved public source at
 * ctrdecomp/NintendoWare4Ctr commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified 2026-10-20: shared native-value lookup callback contract.
 */
#include "game/graphics_resource_abi.h"

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
