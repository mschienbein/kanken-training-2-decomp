#include <preinclude.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Layout.h>
#include "game/layout.h"

static nw::lyt::AnimTransform *create_animation(nw::lyt::Layout *layout,
    const nw::lyt::AnimResource *resource, nw::lyt::ResourceAccessor *accessor)
{
    return layout->CreateAnimTransform(*resource, accessor);
}

/* Native re-expression of the retained pointer-based attachment helper. */
nw::lyt::AnimTransform *func_001ce8b8(nw::lyt::Layout *layout, const void *file,
                                    nw::lyt::ResourceAccessor *accessor)
{
    nw::lyt::AnimResource reader(file);
    return create_animation(layout, &reader, accessor);
}
