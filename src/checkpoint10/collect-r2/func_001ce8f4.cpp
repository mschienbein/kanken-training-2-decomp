/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr contributors at
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Selective canonical-header author candidate; GNU GPL version 3.
 */
#include <preinclude.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Animation.h>
namespace nw { namespace lyt {
AnimTransform *Layout::CreateAnimTransform(const AnimResource &resource, ResourceAccessor *accessor)
{
    const res::AnimationBlock *block = resource.GetResourceBlock();
    if (!block)
        return 0;
    AnimTransform *animation = CreateAnimTransform();
    if (animation)
        animation->SetResource(block, accessor);
    return animation;
}
} }
