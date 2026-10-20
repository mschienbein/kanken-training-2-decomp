#include <preinclude.h>
#include <nw/lyt/lyt_Pane.h>
#include "game/command_buffer.h"

#include "game/layout_draw.h"

static unsigned char* draw_root(LayoutCommandRoot* root, unsigned char* cursor,
    LayoutRenderContext* renderer, LayoutDrawInfo* info)
{
    if (!root)
        return cursor;
    nw::lyt::Pane* pane = root->root;
    if (!pane || !pane->IsVisible())
        return cursor;
    return pane->DrawCommands(cursor, info, renderer);
}

void func_00196c04(LayoutRenderContext* renderer, LayoutCommandRoot* root,
    LayoutDrawInfo* info)
{
    unsigned char* cursor = func_0018d8d4();
    cursor = draw_root(root, cursor, renderer, info);
    func_0018d8c4(cursor);
}
