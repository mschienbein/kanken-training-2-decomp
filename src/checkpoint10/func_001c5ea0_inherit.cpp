/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr, gfx_FragmentLight.cpp and
 * gfx_TransformNode.h, commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 */
#include <preinclude.h>
#include "nw/gfx/gfx_FragmentLight.h"

static void inherit_direction(nw::gfx::FragmentLight *node,
                              const nw::math::VEC3 &direction)
{
    nw::math::VEC3 &output = node->Direction();
    if (node->GetParent() == 0) {
        output = direction;
    } else {
        const nn::math::MTX34 &matrix = node->TrackbackWorldMatrix();
        output.x = matrix.f._00 * direction.x + matrix.f._01 * direction.y + matrix.f._02 * direction.z;
        output.y = matrix.f._10 * direction.x + matrix.f._11 * direction.y + matrix.f._12 * direction.z;
        output.z = matrix.f._20 * direction.x + matrix.f._21 * direction.y + matrix.f._22 * direction.z;
    }
}

/* INTEGRATOR: move to include/game/model_transform.h */
extern "C" {
void func_001c5ea0(nw::gfx::FragmentLight *node);
}

void func_001c5ea0(nw::gfx::FragmentLight *node)
{
    nw::gfx::ResFragmentLight light = node->GetResFragmentLight();
    if (nw::ut::CheckFlag(light.GetFlags(), nw::gfx::ResFragmentLightData::FLAG_IS_INHERITING_DIRECTION_ROTATE))
        inherit_direction(node, light.GetDirection());
    else
        node->Direction() = light.GetDirection();
}
