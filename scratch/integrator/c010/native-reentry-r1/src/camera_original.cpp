/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr, commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd (NintendoWare4Ctr contributors).
 * Modified 2026-10-20: select native storage provider; use tagged resource casts;
 * omitted unrelated definitions. See the repository LICENSE.
 */
#include <preinclude.h>
#include <nw/gfx/gfx_Camera.h>

namespace nw { namespace gfx {

Result Camera::StoreOriginal(nw::os::IAllocator* allocator)
{
    Result result = INITIALIZE_RESULT_OK;

    ResCamera original = GetResCamera();
    NW_NULL_ASSERT(original.GetViewUpdater().ptr());
    NW_NULL_ASSERT(original.GetProjectionUpdater().ptr());

    void* cameraMemory = allocator->Alloc(sizeof(ResCameraData));
    NW_NULL_ASSERT(cameraMemory);

    /* ORIGINAL BUG: allocation failure is unchecked before cameraData is dereferenced. */
    ResCameraData* cameraData = new(cameraMemory) ResCameraData(original.ref());

    cameraData->toViewUpdater = NULL;
    cameraData->toProjectionUpdater = NULL;

    void* viewUpdaterMemory = NULL;
    ResCameraViewUpdaterData* viewUpdaterData = NULL;
    switch (original.GetViewType())
    {
    case ResCamera::VIEWTYPE_AIM:
        viewUpdaterMemory = allocator->Alloc(sizeof(ResAimTargetViewUpdaterData));
        NW_NULL_ASSERT(viewUpdaterMemory);
        viewUpdaterData = new(viewUpdaterMemory) ResAimTargetViewUpdaterData(
            nw::ut::ResStaticCast<ResAimTargetViewUpdater>(original.GetViewUpdater()).ref());
        break;

    case ResCamera::VIEWTYPE_LOOKAT:
        viewUpdaterMemory = allocator->Alloc(sizeof(ResLookAtTargetViewUpdaterData));
        NW_NULL_ASSERT(viewUpdaterMemory);
        viewUpdaterData = new(viewUpdaterMemory) ResLookAtTargetViewUpdaterData(
            nw::ut::ResStaticCast<ResLookAtTargetViewUpdater>(original.GetViewUpdater()).ref());
        break;

    case ResCamera::VIEWTYPE_ROTATE:
        viewUpdaterMemory = allocator->Alloc(sizeof(ResRotateViewUpdaterData));
        NW_NULL_ASSERT(viewUpdaterMemory);
        viewUpdaterData = new(viewUpdaterMemory) ResRotateViewUpdaterData(
            nw::ut::ResStaticCast<ResRotateViewUpdater>(original.GetViewUpdater()).ref());
        break;

    default:
        NW_ASSERT(false);
    }

    cameraData->toViewUpdater.set_ptr(viewUpdaterData);

    void* projectionUpdaterMemory = NULL;
    ResCameraProjectionUpdaterData* projectionUpdaterData = NULL;

    switch (original.GetProjectionType())
    {
    case ResCamera::PROJTYPE_PERSPECTIVE:
        projectionUpdaterMemory = allocator->Alloc(sizeof(ResPerspectiveProjectionUpdaterData));
        NW_NULL_ASSERT(projectionUpdaterMemory);
        projectionUpdaterData = new(projectionUpdaterMemory) ResPerspectiveProjectionUpdaterData(
            nw::ut::ResStaticCast<ResPerspectiveProjectionUpdater>(original.GetProjectionUpdater()).ref());
        break;

    case ResCamera::PROJTYPE_FRUSTUM:
        projectionUpdaterMemory = allocator->Alloc(sizeof(ResFrustumProjectionUpdaterData));
        NW_NULL_ASSERT(projectionUpdaterMemory);
        projectionUpdaterData = new(projectionUpdaterMemory) ResFrustumProjectionUpdaterData(
            nw::ut::ResStaticCast<ResFrustumProjectionUpdater>(original.GetProjectionUpdater()).ref());
        break;

    case ResCamera::PROJTYPE_ORTHO:
        projectionUpdaterMemory = allocator->Alloc(sizeof(ResOrthoProjectionUpdaterData));
        NW_NULL_ASSERT(projectionUpdaterMemory);
        projectionUpdaterData = new(projectionUpdaterMemory) ResOrthoProjectionUpdaterData(
            nw::ut::ResStaticCast<ResOrthoProjectionUpdater>(original.GetProjectionUpdater()).ref());
        break;

    default:
        NW_ASSERT(false);
    }

    cameraData->toProjectionUpdater.set_ptr(projectionUpdaterData);

    m_OriginalValue = ResCamera(cameraData);

    m_OriginalTransform = GetResTransformNode().GetTransform();

    return result;
}

} }
