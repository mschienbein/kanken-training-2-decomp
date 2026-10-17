/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; adapted from the preserved public source at
 * ctrdecomp/NintendoWare4Ctr commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified 2026-10-17: selected definitions, canonical project headers, and
 * existing-address dispatch declarations. No original code bytes are embedded.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResLookupTable.h>

/* INTEGRATOR: move the typed dispatch declaration to the canonical lookup header. */
extern "C" {
extern void (*data_002434e4[2])(nw::gfx::res::ResLookupTable object);
}

namespace nw { namespace gfx { namespace res {

inline void ResLookupTable::Cleanup()
{
    switch (this->ref().typeInfo)
    {
    case ResImageLookupTable::TYPE_INFO:{
            data_002434e4[0](*this);
        }
        break;
    case ResReferenceLookupTable::TYPE_INFO:{
            data_002434e4[1](*this);
        }
        break;
    default:{
            NW_FATAL_ERROR("Unsupported lut type.");
        }
    }
}

void ResLookupTableSet::Cleanup()
{
    /* ORIGINAL BUG: a missing sampler dictionary with a nonzero count still
     * constructs invalid iterator bounds and is traversed without validation. */
    ut::SafeCleanupAll(this->GetSamplers());
}

} } }
