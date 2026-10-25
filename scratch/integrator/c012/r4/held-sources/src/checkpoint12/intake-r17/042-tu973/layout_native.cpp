/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd and the preserved prior candidate.
 * Modified: canonical repository types and recovered retail code structure.
 */
#include "preinclude.h"
#include "game/font_resource.h"
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Bounding.h>
#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_DrawInfo.h>
#include <nw/lyt/lyt_GraphicsResource.h>
#include <nw/lyt/lyt_Group.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Picture.h>
#include <nw/lyt/lyt_ResourceAccessor.h>
#include <nw/lyt/lyt_Resources.h>
#include <nw/lyt/lyt_TextBox.h>
#include <nw/lyt/lyt_Util.h>
#include <nw/lyt/lyt_Window.h>
namespace nw { namespace lyt {
static void *allocate_layout_object(u32 size, u8 alignment)
{
    return Layout::GetAllocator()->Alloc(size, alignment);
}
template<typename T> static T *new_layout_object()
{
    void *memory = allocate_layout_object(sizeof(T), 4);
    return memory ? new (memory) T() : 0;
}
template<typename T, typename A, typename B> static T *new_layout_object(const A& first, const B& second)
{
    void *memory = allocate_layout_object(sizeof(T), 4);
    return memory ? new (memory) T(first, second) : 0;
}
bool Layout::Build(const void* lytResBuf,ResourceAccessor* pResAcsr)
{
    const ut::BinaryFileHeader *const pFileHead = static_cast<const ut::BinaryFileHeader*>(lytResBuf);

    /* ORIGINAL BUG: retail ignores the resource-validation result. */
    func_001813a0(reinterpret_cast<const BinaryFileHeader *>(pFileHead), 0x54594c43, 0x02020000, 1);
    ResBlockSet resBlockSet = { 0 };
    resBlockSet.pResAccessor = pResAcsr;
    Pane* pParentPane = 0;
    Pane* pLastPane = 0;

    bool bReadRootGroup = false;
    int groupNestLevel = 0;

    const void* dataPtr = static_cast<const u8*>(lytResBuf) + pFileHead->headerSize;
    for (int i = 0; i < pFileHead->dataBlocks; ++i)
    {
        const ut::BinaryBlockHeader* pDataBlockHead = static_cast<const ut::BinaryBlockHeader*>(dataPtr);
        ut::SigWord kind = pDataBlockHead->kind;
        switch (kind)
        {
        case res::DATABLOCKKIND_LAYOUT:
        {
                const res::Layout* pResLyt = static_cast<const res::Layout*>(dataPtr);
                this->SetLayoutSize(pResLyt->layoutSize);
            }
            break;

        case res::DATABLOCKKIND_TEXTURELIST:
            resBlockSet.pTextureList = static_cast<const res::TextureList*>(dataPtr);
            break;

        case res::DATABLOCKKIND_FONTLIST:
            resBlockSet.pFontList = static_cast<const res::FontList*>(dataPtr);
            break;

        case res::DATABLOCKKIND_MATERIALLIST:
            resBlockSet.pMaterialList = static_cast<const res::MaterialList*>(dataPtr);
            break;

        case res::DATABLOCKKIND_PANE:
        case res::DATABLOCKKIND_PICTURE:
        case res::DATABLOCKKIND_TEXTBOX:
        case res::DATABLOCKKIND_WINDOW:
        case res::DATABLOCKKIND_BOUNDING:
        {
                Pane* pPane = BuildPaneObj(kind, dataPtr, resBlockSet);
                if (pPane)
                {
                    if (this->GetRootPane() == 0)
                    {
                        this->SetRootPane(pPane);
                    }
                    if (pParentPane)
                    {
                        pParentPane->AppendChild(pPane);
                    }

                    pLastPane = pPane;
                }
            }
            break;

        case res::DATABLOCKKIND_USERDATALIST:
            /* ORIGINAL BUG: malformed ordering may dereference a null last pane. */
            pLastPane->SetExtUserDataList(reinterpret_cast<const res::ExtUserDataList*>(pDataBlockHead));
            break;

        case res::DATABLOCKKIND_PANEBEGIN:
            pParentPane = pLastPane;
            break;

        case res::DATABLOCKKIND_PANEEND:
            pLastPane = pParentPane;
            /* ORIGINAL BUG: unmatched pane-end may dereference null. */
            pParentPane = pLastPane->GetParent();
            break;

        case res::DATABLOCKKIND_GROUP:
            if (!bReadRootGroup)
            {
                bReadRootGroup = true;
                this->SetGroupContainer(new_layout_object<GroupContainer>());
            }

            else{

                if (this->GetGroupContainer() && groupNestLevel == 1)
                {
                    if (Group* pGroup = new_layout_object<Group>(reinterpret_cast<const res::Group*>(pDataBlockHead), this->GetRootPane()))
                {
                        this->GetGroupContainer()->AppendGroup(pGroup);
                    }
                }
            }
            break;
        case res::DATABLOCKKIND_GROUPBEGIN:
            groupNestLevel++;
            break;
        case res::DATABLOCKKIND_GROUPEND:
            groupNestLevel--;
            break;
        default:
            break;
        }
        dataPtr = static_cast<const u8*>(dataPtr) + pDataBlockHead->size;
    }
    return true;
}

}}
