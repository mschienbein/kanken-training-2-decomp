/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/gfx_SceneHelper.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "gfx_SceneNode.h"
#include "gfx_Camera.h"
#include "../ut/ut_MoveArray.h"
#include "../math/math_Types.h"

namespace nw{
namespace gfx{

class SceneHelper
{
private:
    SceneHelper();
    NW_DISALLOW_COPY_AND_ASSIGN(SceneHelper);

public:
    template<typename TIterator>
    static void ResolveReference(TIterator begin, TIterator end)
    {
        for (TIterator parent = begin; parent != end; ++parent)
        {
            for (TIterator child = begin; child != end; ++child)
            {
                NW_NULL_ASSERT(nw::ut::DynamicCast<SceneNode*>(*child));
                ResolveReferenceImpl(*parent, *child);
            }
        }
    }

    template<typename TIterator>
    static void ResolveReference(const std::pair<TIterator, TIterator>& range)
    {
        ResolveReference<TIterator>(range.first, range.second);
    }

    template<typename TNode, int TSize>
    static void ResolveReference(TNode (&nodes)[TSize])
    {
        ResolveReference<TNode*>(nodes, nodes + TSize);
    }

    template<typename TNodeArray>
    static void ResolveReference(TNodeArray& nodes)
    {
        ResolveReference<TNodeArray::iterator>(nodes.begin(), nodes.end());
    }

    template <typename TIterator, typename TFunction>
    static void
    ForeachRootNodes(TIterator begin, TIterator end, TFunction function)
    {
        for (TIterator iter = begin; iter != end; ++iter)
        {
            if ((*iter)->GetParent() == NULL)
            {
                function(*iter);
            }
        }
    }

    static float CalculateDepth(
        const math::VEC3& localPosition,
        const math::MTX34& worldMatrix,
        const Camera& camera)
        {
        math::VEC3 position;
        math::VEC3Transform(&position, &worldMatrix, &localPosition);
        math::VEC3Transform(&position, &camera.ViewMatrix(), &position);
        const math::MTX44& projection = camera.ProjectionMatrix();
        float z =
            projection.m[2][0] * position.x +
            projection.m[2][1] * position.y +
            projection.m[2][2] * position.z +
            projection.m[2][3];
        float w =
            projection.m[3][0] * position.x +
            projection.m[3][1] * position.y +
            projection.m[3][2] * position.z +
            projection.m[3][3];
        return ut::Clamp(-z / w, 0.0f, 1.0f);
    }

    static float CalculateDepth(
        const math::MTX34& worldMatrix,
        const Camera& camera)
        {
        math::VEC3 position(worldMatrix.GetColumn(3));
        math::VEC3Transform(&position, (const math::MTX34*)&camera.ViewMatrix(), &position);
        const math::MTX44& projection = camera.ProjectionMatrix();
        float z =
            projection.m[2][0] * position.x +
            projection.m[2][1] * position.y +
            projection.m[2][2] * position.z +
            projection.m[2][3];
        float w =
            projection.m[3][0] * position.x +
            projection.m[3][1] * position.y +
            projection.m[3][2] * position.z +
            projection.m[3][3];
        return nw::ut::Clamp(-z / w, 0.0f, 1.0f);
    }
private:
    static void ResolveReferenceImpl(SceneNode* parent, SceneNode* child);
};

class AttachNode
{
public:
    AttachNode(gfx::SceneNode* parent): 
        m_Parent(parent) {}

    template <typename TNode>
    void operator() (TNode* node)
    {
        NW_POINTER_ASSERT(this->m_Parent);
        this->m_Parent->AttachChild(node);
    }
    
private:
    gfx::SceneNode* m_Parent;
};

}
}

