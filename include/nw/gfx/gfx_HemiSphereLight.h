/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_HemiSphereLight.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_Light.h"
#include "res/gfx_ResLight.h"

#include "../ut/ut_MovePtr.h"
#include <functional>

namespace nw{
namespace gfx{

class HemiSphereLight : public Light
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(HemiSphereLight);

public:
    NW_UT_RUNTIME_TYPEINFO;

    struct Description : public Light::Description
    {
        Description() {}
    };

    class DynamicBuilder
    {
    public:
        DynamicBuilder() {}
        ~DynamicBuilder() {}

        DynamicBuilder& IsFixedSizeMemory(bool isFixedSizeMemory)
        {
            m_Description.isFixedSizeMemory = isFixedSizeMemory;
            return *this;
        }

        DynamicBuilder& MaxChildren(int maxChildren)
        {
            m_Description.maxChildren = maxChildren;
            return *this;
        }

        DynamicBuilder& MaxCallbacks(int maxCallbacks)
        {
            m_Description.maxCallbacks = maxCallbacks;
            return *this;
        }

        HemiSphereLight* Create(nw::os::IAllocator* allocator);

       size_t GetMemorySize(size_t alignment = nw::os::IAllocator::DEFAULT_ALIGNMENT) const;

    private:
        HemiSphereLight::Description m_Description;
    };

    static HemiSphereLight* Create(SceneNode* parent,ResSceneObject resource,const HemiSphereLight::Description& description,nw::os::IAllocator* allocator);

    static size_t GetMemorySize(ResHemiSphereLight resource,Description description,size_t alignment = nw::os::IAllocator::DEFAULT_ALIGNMENT)
    {
        nw::os::MemorySizeCalculator size(alignment);

        GetMemorySizeInternal(&size, resource, description);

        return size.GetSizeWithPadding(alignment);
    }

    static void GetMemorySizeInternal( nw::os::MemorySizeCalculator* pSize,ResHemiSphereLight resource,Description description);

    virtual void Accept(ISceneVisitor* visitor);

    ResHemiSphereLight GetResHemiSphereLight()
    {
        return ResStaticCast<ResHemiSphereLight>(this->GetResSceneObject());
    }

    const ResHemiSphereLight GetResHemiSphereLight() const
    {
        return ResStaticCast<ResHemiSphereLight>(this->GetResSceneObject());
    }

protected:
    struct ResHemiSphereLightDataDestroyer : public std::unary_function<ResHemiSphereLightData*, void>
    {
        ResHemiSphereLightDataDestroyer(nw::os::IAllocator* allocator = 0): 
            m_Allocator(allocator) 
        {
        }

        result_type operator()(argument_type data)
        {
            DestroyResHemiSphereLight(m_Allocator, data);
        }

        nw::os::IAllocator* m_Allocator;
    };

    typedef nw::ut::MovePtr<ResHemiSphereLightData, ResHemiSphereLightDataDestroyer> ResPtr;

    HemiSphereLight(nw::os::IAllocator* allocator,ResHemiSphereLight resObj,const HemiSphereLight::Description& description): 
        Light(allocator, resObj, description) 
    {
    }

    HemiSphereLight(os::IAllocator* allocator, ResPtr resource, const HemiSphereLight::Description& description): 
        Light(allocator, ResHemiSphereLight(resource.Get()), description),
        m_Resource(resource)
    {
    }

    virtual ~HemiSphereLight()
    {
        this->DestroyOriginalValue();
    }
    
private:
    virtual Result Initialize(nw::os::IAllocator* allocator);

    static ResHemiSphereLightData*    CreateResHemiSphereLight(nw::os::IAllocator* allocator, const char* name = NULL);

    static void DestroyResHemiSphereLight(nw::os::IAllocator* allocator, ResHemiSphereLightData* resHemiSphereLight);

    Result CreateOriginalValue(nw::os::IAllocator* allocator);

    virtual u32 GetLightType() const
    {
        return anim::ResLightAnimData::LIGHT_TYPE_HEMISPHERE;
    }

    virtual u32 GetLightKind() const
    {
        return ResLight::KIND_UNUSED;
    }

    ResPtr m_Resource;
};

}
}

