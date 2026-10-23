#define NW_RELEASE
#include <string.h>
#include "game/pane_tree.h"

static __forceinline DisplayPart *const *display_parts(const DisplayData *object)
{ return reinterpret_cast<DisplayPart *const *>(object + 1); }

static nw::lyt::Pane *first_child(nw::lyt::Pane *pane)
{
    const nw::lyt::PaneList &children = pane->GetChildList();
    nw::lyt::Pane *child = 0;
    if (!children.IsEmpty())
        child = const_cast<nw::lyt::Pane *>(&children.front());
    return child;
}

void func_0017053c(const struct DisplayData *object, const char *name, float *width, float *height)
{
    nw::lyt::Pane *root = display_parts(object)[0]->container->root;
    nw::lyt::Pane *pane = first_child(root);
    if (name != 0)
        pane = func_00167ee4(root, name);
    if (pane != 0) {
        nw::lyt::Size size;
        memcpy(&size, &pane->GetSize(), sizeof size);
        memcpy(width, &size.width, sizeof *width);
        memcpy(height, &size.height, sizeof *height);
        return;
    }
    *width = 0.0f;
    *height = 0.0f;
}
