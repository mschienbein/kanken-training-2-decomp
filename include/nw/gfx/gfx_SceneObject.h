/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_SceneObject.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_Common.h"
#include "gfx_GfxObject.h"
#include "res/gfx_ResSceneObject.h"
#include "../ut/ut_ResUtil.h"
#include "../ut/ut_ResDictionary.h"
#include "../ut/ut_RuntimeTypeInfo.h"
#include "../ut/ut_MovePtr.h"
#include "../ut/ut_Children.h"
#include "../os/os_Memory.h"

namespace nw{
namespace gfx{

class SceneNode;
class TransformNode;

typedef nw::ut::Children<SceneNode, SceneNode, nw::ut::ChildDetacher<SceneNode> > SceneNodeChildren;

class SceneObject : public GfxObject
{
private:
    NW_DISALLOW_COPY_AND_ASSIGN(SceneObject);

public:
    NW_UT_RUNTIME_TYPEINFO;

    static const int DEFAULT_MAX_CHILDREN = 8;
    static const int DEFAULT_MAX_CALLBACKS = 4;
    static const int MAX_NAME_LENGTH = 256;

    ResSceneObject GetResSceneObject() { return m_ResObject; }
    const ResSceneObject GetResSceneObject() const { return m_ResObject; }

    const char* GetName() const
    {
        NW_ASSERT(this->m_ResObject.IsValid());
        return this->m_ResObject.GetName();
    }
    
protected:
    SceneObject(nw::os::IAllocator* allocator, ResSceneObject resObj): 
        GfxObject(allocator), 
        m_ResObject(resObj) {}

    virtual ~SceneObject() {}

private:
    ResSceneObject  m_ResObject;
};

}
}

