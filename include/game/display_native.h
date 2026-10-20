#ifndef GAME_DISPLAY_NATIVE_H
#define GAME_DISPLAY_NATIVE_H

#include "game/screen.h"
#include "nw/lyt/lyt_Picture.h"
#include "nw/lyt/lyt_TextBox.h"
#include "nw/lyt/lyt_Layout.h"

extern "C" {
void func_0014d000(DisplayObject *object, const nw::math::VEC2 *coordinates);
void func_0014d02c(nw::lyt::Picture *pane, unsigned index,
                   const nw::math::VEC2 *coordinates);
void func_0014e504(nw::lyt::Picture *pane, unsigned count);
void func_0014e50c(nw::lyt::internal::TexCoordAry *vertices, unsigned count);
void func_0014e65c(nw::lyt::internal::TexCoordAry *vertices, unsigned index,
                   const nw::math::VEC2 *coordinates);
}

#endif
