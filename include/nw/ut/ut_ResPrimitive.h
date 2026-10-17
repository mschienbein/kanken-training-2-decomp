/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/ut/ut_ResPrimitive.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once


#include "ut_ResTypes.h"
#include "ut_Inlines.h"
#include "ut_ResUtil.h"
#include "ut_ResDeclMacros.h"
#include "ut_ResTypeInfo.h"
#include "../math/math_Types.h"

namespace nw {
namespace ut {

struct ResBoundingVolumeData
{
    nw::ut::ResTypeInfo typeInfo;
};

struct ResOrientedBoundingBoxData : public ResBoundingVolumeData
{
    ResVec3   m_CenterPosition;
    ResMtx33  m_OrientationMatrix;
    ResVec3   m_Size;
};

struct ResAxisAlignedBoundingBoxData : public ResBoundingVolumeData
{
    ResVec3   m_CenterPosition;
    ResVec3   m_Size;
};

class ResBoundingVolume : public nw::ut::ResCommon<ResBoundingVolumeData>
{
public:
    enum{ TYPE_INFO = NW_UT_RES_TYPE_INFO(ResBoundingVolume) };
    
    NW_RES_CTOR( ResBoundingVolume )

    nw::ut::ResTypeInfo     GetTypeInfo() const { return ref().typeInfo; }
};

class ResOrientedBoundingBox : public ResBoundingVolume
{
public:
    enum{ TYPE_INFO = NW_UT_RES_TYPE_INFO(ResOrientedBoundingBox) };
    
    NW_RES_CTOR_INHERIT(ResOrientedBoundingBox, ResBoundingVolume )

    NW_RES_FIELD_VECTOR3_DECL(nw::math::VEC3, CenterPosition)
    
    NW_RES_FIELD_STRUCT_DECL(nw::math::MTX33, OrientationMatrix)

    NW_RES_FIELD_VECTOR3_DECL(nw::math::VEC3, Size)
};

class ResAxisAlignedBoundingBox : public ResBoundingVolume
{
public:
    enum
{ TYPE_INFO = NW_UT_RES_TYPE_INFO(ResAxisAlignedBoundingBox) };
    
    NW_RES_CTOR_INHERIT(ResAxisAlignedBoundingBox, ResBoundingVolume)

    NW_RES_FIELD_VECTOR3_DECL(nw::math::VEC3, CenterPosition)
    
    NW_RES_FIELD_VECTOR3_DECL(nw::math::VEC3, Size)
};

}
}

