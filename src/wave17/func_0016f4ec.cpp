#include <preinclude.h>
#include <string.h>
#include "game/screen.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Pane.h"

extern "C" void func_0016f4ec(const DisplayObject *object, float *width, float *height)
{
    const nw::lyt::PaneList &list = display_layout_attachment(object, 0)->layout->GetRootPane()->GetChildList();
    const nw::lyt::Pane *node = 0;
    if (!list.empty())
        node = &list.front();
    if (node) {
        float size[2];
        memcpy(&size[0], &node->m_Size.width, sizeof size[0]);
        memcpy(&size[1], &node->m_Size.height, sizeof size[1]);
        memcpy(width, &size[0], sizeof *width);
        memcpy(height, &size[1], sizeof *height);
    } else {
        *width = 0.0f;
        *height = 0.0f;
    }
}
