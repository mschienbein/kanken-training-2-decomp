/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_WaveSoundFile.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_WaveSoundFile.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_WaveSoundFile.h>
#include <nw/ut/ut_Inlines.h>

namespace nw {
namespace snd {
namespace internal {

namespace {

const u8 DEFAULT_PAN = 64;
const s8 DEFAULT_SURROUND_PAN = 0;
const f32 DEFAULT_PITCH = 1.0f;
const u8 DEFAULT_MAIN_SEND = 127;
const u8 DEFAULT_FX_SEND = 0;
extern const AdshrCurve DEFAULT_ADSHR_CURVE;
const u8 DEFAULT_KEY  = 64;
const u8 DEFAULT_VOLUME         = 96;

enum WaveSoundInfoBitFlag
{
    WAVE_SOUND_INFO_PAN = 0x00,
    WAVE_SOUND_INFO_PITCH,
    WAVE_SOUND_INFO_SEND = 0x08,
    WAVE_SOUND_INFO_ENVELOPE,
    WAVE_SOUND_INFO_RANDOMIZER
};

enum NoteInfoBitFlag
{
    NOTE_INFO_KEY = 0x00,
    NOTE_INFO_VOLUME,
    NOTE_INFO_PAN,
    NOTE_INFO_PITCH,
    NOTE_INFO_SEND = 0x08,
    NOTE_INFO_ENVELOPE,
    NOTE_INFO_RANDOMIZER,
    NOTE_INFO_LFO
};

struct SendValue
{
    u8 mainSend;
    Util::Table<u8,u8> fxSend;
};

} // namespace

// WaveSoundFile::FileHeader

// WaveSoundFile::InfoBlockBody

// WaveSoundFile::WaveSoundData

const Util::ReferenceTable& WaveSoundFile::WaveSoundData::GetNoteInfoReferenceTable() const
{
    return *reinterpret_cast<const Util::ReferenceTable*>(ut::AddOffsetToPtr(this, toNoteInfoReferenceTable.offset));
}

// WaveSoundFile::WaveSoundInfo

// WaveSoundFile::TrackInfo

// WaveSoundFile::NoteInfo

} // namespace internal
} // namespace snd
} // namespace nw

