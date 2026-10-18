/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/lyt/lyt_Animation.cpp; Set plus its inline initialization helper.
 */
#include <preinclude.h>
#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Resources.h>
#include <nw/ut/ut_BinaryFileFormat.h>
namespace nw { namespace lyt {
inline void AnimResource::Init()
{
    m_pFileHeader = NULL;
    m_pResBlock = NULL;
    m_pTagBlock = NULL;
    m_pShareBlock = NULL;
}
void AnimResource::Set(const void* anmResBuf)
{
    this->Init();
    const ut::BinaryFileHeader *const pFileHeader = static_cast<const ut::BinaryFileHeader*>(anmResBuf);
    if (!ut::IsValidBinaryFile(pFileHeader, res::FILESIGNATURE_CLAN, res::BinaryFileFormatVersion))
        return;
    m_pFileHeader = pFileHeader;
    const ut::BinaryBlockHeader* pDataBlockHead = internal::ConvertOffsToPtr<ut::BinaryBlockHeader>(pFileHeader, pFileHeader->headerSize);
    for (int i = 0; i < pFileHeader->dataBlocks; ++i) {
        SigWord kind = pDataBlockHead->kind;
        switch (kind) {
        case res::DATABLOCKKIND_PANEANIMTAG:
            m_pTagBlock = reinterpret_cast<const res::AnimationTagBlock*>(pDataBlockHead);
            break;
        case res::DATABLOCKKIND_PANEANIMSHARE:
            m_pShareBlock = reinterpret_cast<const res::AnimationShareBlock*>(pDataBlockHead);
            break;
        case res::DATABLOCKKIND_PANEANIMINFO:
            m_pResBlock = reinterpret_cast<const res::AnimationBlock*>(pDataBlockHead);
            break;
        }
        pDataBlockHead = internal::ConvertOffsToPtr<ut::BinaryBlockHeader>(pDataBlockHead, pDataBlockHead->size);
    }
}
} }
