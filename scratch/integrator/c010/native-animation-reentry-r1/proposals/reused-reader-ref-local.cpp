#include <preinclude.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Layout.h>
#include "game/layout.h"

/* Native re-expression of the retained reader/ref-local source shape. */
nw::lyt::AnimTransform *func_001ce8b8(nw::lyt::Layout *layout, const void *file,
                                    nw::lyt::ResourceAccessor *accessor)
{
    nw::lyt::AnimResource reader(file);
    nw::lyt::AnimResource *resource = &reader;
    const nw::lyt::AnimResource &argument = *resource;
    return layout->CreateAnimTransform(argument, accessor);
}
