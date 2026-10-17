/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_SortingMaterialIdGenerator.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_IMaterialIdGenerator.h"

namespace nw{
namespace gfx{
namespace internal{
    struct MaterialKeyValue
    {
        u32 uniqueId;
        u32 key;
        u32 subKey;
        Material* material;

    };
}

class SortingMaterialIdGenerator : public IMaterialIdGenerator
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(SortingMaterialIdGenerator);

    typedef nw::ut::MoveArray<internal::MaterialKeyValue> MaterialKeyValueArray;
public:
    NW_UT_RUNTIME_TYPEINFO;
    
    struct Description
{        
        bool isFixedSizeMemory;
        int  maxMaterials;

        Description(): 
            isFixedSizeMemory(true),
            maxMaterials(128) {}
    };

    class Builder
    {
    public:
        Builder& IsFixedSizeMemory(bool isFixedSizeMemory)
        {
            m_Description.isFixedSizeMemory = isFixedSizeMemory;
            return *this;
        }

        Builder& MaxMaterials(int maxMaterials)
        {
            m_Description.maxMaterials = maxMaterials;
            return *this;
        }

        IMaterialIdGenerator* Create(nw::os::IAllocator* allocator);

    private:
        Description m_Description;
    };

    virtual void Accept(Material* material);

    virtual void Generate();

private:

    SortingMaterialIdGenerator(nw::os::IAllocator* allocator,MaterialKeyValueArray materials,MaterialKeyValueArray materialsWorkSpace): 
        IMaterialIdGenerator(allocator),
        m_Materials(materials),
        m_MaterialsWorkSpace(materialsWorkSpace) {}

    MaterialKeyValueArray m_Materials;
    MaterialKeyValueArray m_MaterialsWorkSpace;
};

}
}

