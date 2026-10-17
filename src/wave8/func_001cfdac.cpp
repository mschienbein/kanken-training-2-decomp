#include "game/layout.h"

extern "C" LayoutAnimationLink *func_001cfdac(
    const LayoutMaterial *material, const LayoutAnimationTransform *animation)
{
    return func_001cff58(&material->animation_links, animation);
}
