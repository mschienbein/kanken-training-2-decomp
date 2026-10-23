#include <preinclude.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Layout.h>
#include "game/layout.h"

nw::lyt::AnimTransform *func_001ce8b8(nw::lyt::Layout *layout, const void *file,
                                    nw::lyt::ResourceAccessor *accessor)
{
    const nw::lyt::AnimResource &resource = nw::lyt::AnimResource(file);
    return layout->CreateAnimTransform(resource, accessor);
}
