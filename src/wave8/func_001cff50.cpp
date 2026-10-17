#include "game/layout.h"

extern "C" LayoutAnimationLink *func_001cff50(
    const LayoutPane *pane, const LayoutAnimationTransform *animation)
{
    return func_001cff58(&pane->animation_links, animation);
}
