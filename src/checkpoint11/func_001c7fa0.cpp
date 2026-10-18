/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr, gfx/res/gfx_ResUtil.cpp,
 * commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Replaces the earlier inferred raw-pointer and displacement reconstruction
 * with the canonical resource views and native pair result. */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResUtil.h>
#include <nw/gfx/res/gfx_ResGraphicsFile.h>
namespace nw { namespace gfx { namespace res {
::std::pair<ResTexture, bool>
GetReferenceTextureTarget(ResReferenceTexture referenceTexture, ResGraphicsFile graphicsFile)
{
    bool isExisted = false;
    const char* path = referenceTexture.GetPath();
    ResTexture resTexture = graphicsFile.GetTextures(path);
    if (resTexture.IsValid()) {
        ResReferenceTexture refer = ResDynamicCast<ResReferenceTexture>(resTexture);
        if (refer.IsValid()) {
            ::std::pair<ResTexture, bool> referenceResult;
            referenceResult = GetReferenceTextureTarget(refer, graphicsFile);
            if (referenceResult.second) {
                referenceTexture.ref().toTargetTexture.set_ptr(reinterpret_cast<const void*>(referenceResult.first.ptr()));
                isExisted = true;
            }
        } else {
            referenceTexture.ref().toTargetTexture.set_ptr(reinterpret_cast<const void*>(resTexture.ptr()));
            isExisted = true;
        }
    }
    return ::std::make_pair(referenceTexture.GetTargetTexture(), isExisted);
}
} } }
