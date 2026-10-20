#ifndef GAME_PANE_TREE_H
#define GAME_PANE_TREE_H

#include "game/screen.h"
#include "game/layout_resources.h"

/* [INFERENCE] The dimension/name clients share this actual native Pane root.
 * Its child count and intrusive sentinel are not a pointer-valued C list root.
 * No LayoutTreeHeader-to-Pane alias or containing-object cast is used. */
struct LayoutContainer {
    unsigned char reserved00[0x10];
    nw::lyt::Pane *root;
};

extern "C" {
void func_0016f4ec(const DisplayData *object, float *width, float *height);
void func_0017053c(const DisplayData *object, const char *name,
                   float *width, float *height);
}

#endif
