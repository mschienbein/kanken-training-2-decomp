/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_Util.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "lyt_Types.h"

namespace nw {
namespace lyt {

class Pane;
class Layout;
class DrawInfo;
class Group;
class AnimTransform;
class TexMap;

void BindAnimation(Group* pGroup, AnimTransform* pAnimTrans, bool bRecursive = true, bool bDisable = false);
void UnbindAnimation(Group* pGroup, AnimTransform* pAnimTrans, bool bRecursive = true);
void SetAnimationEnable(Group* pGroup, AnimTransform* pAnimTrans, bool bEnable, bool bRecursive = true);

bool IsContain(Pane* pPane, const nn::math::VEC2& pos);
Pane* FindHitPane(Pane* pPane, const nn::math::VEC2& pos);
Pane* FindHitPane(Layout* pLayout, const nn::math::VEC2& pos);
Pane* GetNextPane(Pane* pPane);

const TextureInfo LoadTexture(const void* pImgRes, u32 size, int texLoadFlag = 0);
void DeleteTexture(const TextureInfo& texInfo);
void CalcTextureMtx(nw::math::MTX23* pTexMtx, const TexSRT& texSRT, const TexMap& texMap);

}
}

