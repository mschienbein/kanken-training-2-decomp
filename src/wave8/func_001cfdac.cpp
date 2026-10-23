#include "game/layout.h"

extern "C" nw::lyt::AnimationLink *func_001cfdac(
    LayoutMaterial *material, nw::lyt::AnimTransform *animation)
{
    return nw::lyt::internal::FindAnimationLink(&material->animation_links, animation);
}
