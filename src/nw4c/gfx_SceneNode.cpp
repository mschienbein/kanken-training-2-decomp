/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/gfx_SceneNode.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: selected C++ definitions and used inline helpers;
 * omitted other definitions/assembly; relative includes/encoding normalized.
 * Class-static storage remains declaration-only at its original address;
 * retained algorithms/tables unchanged. GNU GPL v3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: gfx_SceneNode.cpp
//
// Project: NintendoWare4Ctr

#include <nw/gfx/gfx_SceneNode.h>
#include <nw/gfx/gfx_ISceneVisitor.h>

#include <nw/ut/ut_ResUtil.h>
#include <nw/ut/ut_ResDictionary.h>
#include <nw/ut/ut_Foreach.h>

namespace nw{
namespace gfx{

Result SceneNode::Initialize(os::IAllocator* allocator)
{
    Result result = INITIALIZE_RESULT_OK;

    result |= CreateChildren(allocator);
    NW_ENSURE_AND_RETURN(result);

    this->m_Children.SetParent(this);

    result |= CreateCallbacks(allocator);
    NW_ENSURE_AND_RETURN(result);

    result |= CreateAnimBinding(allocator);
    NW_ENSURE_AND_RETURN(result);

    return result;
}

}
}

