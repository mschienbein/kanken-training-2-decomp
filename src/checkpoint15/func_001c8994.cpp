/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr gfx_ResShape.cpp,
 * commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified: address-named call interfaces; original helper boundaries.
 */
#include "game/geometry_resources.h"
using namespace nw::gfx;

/* [INFERENCE] Private suffix-traversal boundary; canonical native resource views. */
static void cleanup_shape_from(ResShape shape, s32 first)
{
    s32 count = shape.GetPrimitiveSetsCount();
    for (s32 i = first; i < count; ++i) {
        ResPrimitiveSet set = shape.GetPrimitiveSets(i);
        func_001c78f8(&set);
    }
}
extern "C" void func_001c8994(ResShape shape)
{
    int i = 0;
    ResSeparateDataShape separate = nw::ut::ResStaticCast<ResSeparateDataShape>(shape);
    s32 count = separate.GetVertexAttributesCount();
    for (; i < count; ++i) {
        ResVertexAttribute attribute = separate.GetVertexAttributes(i);
        attribute.Cleanup();
    }
    cleanup_shape_from(shape, 0);
}
