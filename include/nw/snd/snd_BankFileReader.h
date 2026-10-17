/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_BankFileReader.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_BANK_FILE_READER_H_
#define NW_SND_BANK_FILE_READER_H_

#include "snd_Global.h"
#include "snd_BankFile.h"
#include "../ut/ut_BinaryFileFormat.h"

namespace nw { 
namespace snd { 
namespace internal {

struct VelocityRegionInfo
{
    u32 waveArchiveId;
    u32 waveIndex;

    f32 pitch;
    AdshrCurve adshrCurve;

    u8 originalKey;
    u8 volume;
    u8 pan;
    bool isIgnoreNoteOff;
    u8 keyGroup;
    u8 interpolationType;
};

class BankFileReader
{
public:
    static const u32 SIGNATURE_FILE = NW_UT_MAKE_SIGWORD('C', 'B', 'N', 'K');

    BankFileReader(const void* bankFile);

    bool ReadVelocityRegionInfo(VelocityRegionInfo* info,int programNo,int key,int velocity) const;

    const Util::WaveIdTable& GetWaveIdTable() const;

    int GetInstrumentCount() const { return m_pInfoBlockBody->GetInstrumentCount(); }
    
private:
    const BankFile::FileHeader*     m_pHeader;
    const BankFile::InfoBlockBody*  m_pInfoBlockBody;
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_BANK_FILE_READER_H_

