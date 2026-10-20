#include <preinclude.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Layout.h>
#include "game/layout.h"

nw::lyt::AnimTransform *func_001ce8b8(nw::lyt::Layout *layout, const void *file,
                                    nw::lyt::ResourceAccessor *accessor)
{
    return layout->CreateAnimTransform(nw::lyt::AnimResource(file), accessor);
}
