/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr, commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd (NintendoWare4Ctr contributors).
 * Modified 2026-10-17: selected native definitions, canonical project headers;
 * omitted unrelated definitions. See the repository LICENSE.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_TransformNode.h>
#include <nw/gfx/res/gfx_ResCamera.h>
#include <new>

/* [INFERENCE] Camera storage view uses canonical resource/value types.
 * The complete canonical Camera header currently lacks PivotDirection.
 */
extern "C" void func_001c9944(nw::os::IAllocator* allocator, nw::gfx::ResCamera resCamera)
{
    NW_NULL_ASSERT(allocator);
    if (resCamera.IsValid())
    {
        if (resCamera.GetProjectionUpdater().IsValid())
        {
            allocator->Free(resCamera.GetProjectionUpdater().ptr());
            resCamera.ref().toProjectionUpdater.set_ptr(NULL);
        }
        if (resCamera.GetViewUpdater().IsValid())
        {
            allocator->Free(resCamera.GetViewUpdater().ptr());
            resCamera.ref().toViewUpdater.set_ptr(NULL);
        }

        allocator->Free(resCamera.ptr());
    }
}
