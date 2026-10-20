/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr, commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd: gfx_AnimGroup.cpp,
 * gfx_AnimGroup.h and ut_MoveArray.h. NintendoWare4Ctr contributors.
 * Modified 2026-10-17: ABI storage view with complete zero initialization,
 * canonical resource/allocator/element types, paired construction/destruction.
 */
#include <preinclude.h>
#include <nw/anim/res/anim_ResAnim.h>
#include <nw/gfx/gfx_AnimGroup.h>
#include <new>

#include "game/render_resource.h"
R41270AnimGroupStorage::~R41270AnimGroupStorage()
{
    vtable = &data_0022409c;
}
