/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; sources/libraries/gfx/gfx_Camera.cpp
 * Pinned upstream: 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified 2026-10-20: select native initializer only; no C receiver/result shim.
 * GNU GPL version 3; see src/nw4c/LICENSE.txt.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_Camera.h>
namespace nw { namespace gfx {
Result Camera::Initialize(nw::os::IAllocator* allocator)
{
    Result result = INITIALIZE_RESULT_OK;
    result |= TransformNode::Initialize(allocator);
    NW_ENSURE_AND_RETURN(result);
    result |= StoreOriginal(allocator);
    result |= CreateAnimGroup(allocator);
    return result;
}
} }
