/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr gfx_ResShape.cpp,
 * commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified: address-named call interfaces; original helper boundaries.
 */
#include "game/geometry_resources.h"
using namespace nw::gfx;

static void setup_primitive_set(ResPrimitiveSet set, nw::os::IAllocator *allocator)
{
    ResPrimitiveArray primitives = set.GetPrimitives();
    for (ResPrimitiveArray::iterator it = primitives.begin(); it != primitives.end(); ++it) {
        ResPrimitive primitive = *it;
        func_001c7560(&primitive, allocator);
    }
}
extern "C" void func_001c8894(nw::os::IAllocator *allocator, ResShape shape)
{
    ResSeparateDataShape separate = nw::ut::ResStaticCast<ResSeparateDataShape>(shape);
    s32 primitiveSetCount = shape.GetPrimitiveSetsCount();
    for (s32 setIndex = 0; setIndex < primitiveSetCount; ++setIndex)
        setup_primitive_set(shape.GetPrimitiveSets(setIndex), allocator);
    s32 count = separate.GetVertexAttributesCount();
    for (int i = 0; i < count; ++i) {
        ResVertexAttribute attribute = separate.GetVertexAttributes(i);
        func_001c7a24(&attribute);
    }
}