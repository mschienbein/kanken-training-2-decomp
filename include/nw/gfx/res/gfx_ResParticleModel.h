/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/res/gfx_ResParticleModel.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../../ut/ut_ResUtil.h"
#include "../../ut/ut_ResDictionary.h"
#include "gfx_ResSceneObject.h"
#include "gfx_ResModel.h"
#include "gfx_ResParticleSet.h"
#include "gfx_ResRevision.h"
#include "gfx_ResTypeInfo.h"

namespace nw {
namespace gfx {
namespace res {

struct ResParticleModelData : public ResModelData
{
    nw::ut::ResS32 m_ParticleSetsTableCount;
    nw::ut::Offset toParticleSetsTable;
};

class ResParticleModel : public ResModel
{
public:
    enum
{ TYPE_INFO = NW_GFX_RES_TYPE_INFO(ResParticleModel) };
    enum
{ SIGNATURE = NW_RES_SIGNATURE32('PMDL') };
    enum
{ BINARY_REVISION = REVISION_RES_EMITTER };

    NW_RES_CTOR_INHERIT(ResParticleModel, ResModel)


    s32 GetParticleSetsCount() const
        { return ref().m_ParticleSetsTableCount; }
    ResParticleSet GetParticleSets(int idx)
        {
            nw::ut::Offset* tbl_ptr = static_cast<nw::ut::Offset*>(ref().toParticleSetsTable.to_table_ptr());
            return ResParticleSet(tbl_ptr[idx].to_ptr());
        }
    const ResParticleSet GetParticleSets(int idx) const
        {
            const nw::ut::Offset* tbl_ptr = static_cast<const nw::ut::Offset*>(ref().toParticleSetsTable.to_table_ptr());
            return ResParticleSet(tbl_ptr[idx].to_ptr());
        }
    nw::ut::internal::ResArray<ResParticleSet, nw::ut::internal::ResArrayClassTraits> GetParticleSets()
        {
            return nw::ut::internal::ResArray<ResParticleSet, nw::ut::internal::ResArrayClassTraits>(ref().toParticleSetsTable.to_table_ptr(), GetParticleSetsCount());
        }
    const nw::ut::internal::ResArray<const ResParticleSet, nw::ut::internal::ResArrayClassTraits> GetParticleSets() const
        {
            return nw::ut::internal::ResArray<const ResParticleSet, nw::ut::internal::ResArrayClassTraits>(ref().toParticleSetsTable.to_table_ptr(), GetParticleSetsCount());
        }

    void Setup();

    template<typename TFunction>
    void ForeachParticleSet(TFunction function)
    {
        if (this->IsValid())
        {
            for (s32 i = 0; i < this->GetParticleSetsCount(); ++i)
            {
                ResParticleSet resParticleSet = this->GetParticleSets(i);
                function(resParticleSet);
            }
        }
    }
};

}
}
}

