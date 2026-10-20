#include <preinclude.h>
#include <string.h>
#include "game/display_native.h"

extern "C" void func_0016f4ec(const DisplayObject *object, float *width, float *height)
{
    const nw::lyt::PaneList &list = display_layout_attachment(object, 0)->layout->GetRootPane()->GetChildList();
    const nw::lyt::Pane *node = 0;
    if (!list.empty())
        node = &list.front();
    if (node) {
        float size[2];
        memcpy(size, &node->m_Size, sizeof size);
        memcpy(width, &size[0], sizeof *width);
        memcpy(height, &size[1], sizeof *height);
    } else {
        *width = 0.0f;
        *height = 0.0f;
    }
}
