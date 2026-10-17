#include "game/layout.h"
#include "game/layout_draw.h"

struct R41253RenderRoot {
    unsigned char reserved00[0x10];
    LayoutDrawPane *root;
};

/* INTEGRATOR: move to include/game/render_commands.h */
extern "C" {
unsigned char *func_0018d8d4(void);
void func_0018d8c4(unsigned char *cursor);
unsigned char *func_00117ea8(LayoutDrawPane *pane, unsigned char *cursor,
                            LayoutDrawInfo *info, LayoutRenderContext *renderer);
void func_00196c04(LayoutRenderContext *renderer, R41253RenderRoot *root,
                   LayoutDrawInfo *info);
}

static unsigned char *draw_root(R41253RenderRoot *root, unsigned char *cursor,
                                LayoutRenderContext *renderer, LayoutDrawInfo *info)
{
    if (!root) return cursor;
    LayoutDrawPane *pane = root->root;
    if (!pane || !(pane->flags & 1)) return cursor;
    return func_00117ea8(pane, cursor, info, renderer);
}

void func_00196c04(LayoutRenderContext *renderer, R41253RenderRoot *root,
                   LayoutDrawInfo *info)
{
    unsigned char *cursor = func_0018d8d4();
    cursor = draw_root(root, cursor, renderer, info);
    func_0018d8c4(cursor);
}
