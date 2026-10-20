#include "game/layout.h"

extern "C" nw::lyt::AnimationLink *func_001cff50(
    LayoutPane *pane, nw::lyt::AnimTransform *animation)
{
    return nw::lyt::internal::FindAnimationLink(&pane->animation_links, animation);
}
