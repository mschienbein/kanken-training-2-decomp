#include "game/layout_resources.h"
extern "C" nw::lyt::Pane *func_00167ee4(nw::lyt::Pane *pane, const char *name)
{
    if (pane == 0)
        return 0;
    nw::lyt::Pane *child = pane->GetFirstChild();
    nw::lyt::Pane *found;
    if (child == 0)
        goto not_found;
search_child:
    if (func_0015dd7c(child->GetName(), name) == 0)
        goto finish;
    found = func_00167ee4(child, name);
    if (found != 0) {
        child = found;
        goto finish;
    }
    child = pane->GetNextChild(child);
    if (child != 0)
        goto search_child;
not_found:
    /* Empty and exhausted child lists share the null-result exit. */
    child = 0;
finish:
    return child;
}
