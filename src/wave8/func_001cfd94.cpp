#include "game/layout.h"

extern "C" void func_001cfd94(LayoutMaterial *material,
                              nw::lyt::AnimTransform *animation)
{
    func_0016b5fc(&material->animation_links, animation);
}
