/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr, commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd (NintendoWare4Ctr contributors).
 * Modified 2026-10-17: selected native definitions, canonical project headers;
 * omitted unrelated definitions. See the repository LICENSE.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_Common.h>

struct R41270CameraStorage;
/* INTEGRATOR: replace the older ModelResource/Heap declarations with the
 * canonical NW4C Result and allocator contracts; Camera receiver identity is
 * evidenced by the selected public algorithms, with its full header blocked by
 * the missing PivotDirection declaration.
 */
extern "C" {
nw::gfx::Result func_001c65ac(R41270CameraStorage*, nw::os::IAllocator*);
nw::gfx::Result func_001c9410(R41270CameraStorage*, nw::os::IAllocator*);
nw::gfx::Result func_001c96c4(R41270CameraStorage*, nw::os::IAllocator*);
}
extern "C" nw::gfx::Result func_001c93c8(R41270CameraStorage* object, nw::os::IAllocator* allocator)
{
    nw::gfx::Result result = nw::gfx::INITIALIZE_RESULT_OK;
    result |= func_001c65ac(object, allocator);
    NW_ENSURE_AND_RETURN(result);
    result |= func_001c9410(object, allocator);
    result |= func_001c96c4(object, allocator);
    return result;
}
