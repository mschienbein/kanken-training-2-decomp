/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_SoundArchiveFile.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_SoundArchiveFile.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_SoundArchiveFile.h>

#include <cstring>
#include <nw/ut/ut_Inlines.h>
#include <nw/snd/snd_ItemType.h>

#include <nn/os.h>

namespace nw {
namespace snd {
namespace internal {

namespace
{
const u32 DEFAULT_STRING_ID = SoundArchive::INVALID_ID;
const PanMode DEFAULT_PAN_MODE = PAN_MODE_DUAL;
const PanCurve DEFAULT_PAN_CURVE = PAN_CURVE_SQRT;
const u8 DEFAULT_PLAYER_PRIORITY = 64;
const u8 DEFAULT_CHANNEL_PRIORITY = 64;
const u8 DEFAULT_ACTOR_PLAYER_ID = 0;
const u8 DEFAULT_IS_RELEASE_PRIORITY_FIX = 0;
const bool DEFAULT_IS_FRONT_BYPASS = false;
const u32 DEFAULT_USER_PARAM = 0xffffffff;
const u32 DEFAULT_EXTRA_USER_PARAM_COUNT = 0;
const u32 DEFAULT_SEQ_START_OFFSET = 0;
const u32 DEFAULT_WARC_WAVE_COUNT = 0;
const u32 DEFAULT_PLAYER_HEAP_SIZE = 0;

enum SoundInfoBitFlag
{
    SOUND_INFO_STRING_ID = 0x00,
    SOUND_INFO_PAN_PARAM,
    SOUND_INFO_PLAYER_PARAM,

    SOUND_INFO_OFFSET_TO_3D_PARAM = 0x08,
    SOUND_INFO_OFFSET_TO_SEND_PARAM,
    SOUND_INFO_OFFSET_TO_MOD_PARAM,

    SOUND_INFO_OFFSET_TO_RVL_PARAM = 0x10,
    SOUND_INFO_OFFSET_TO_CTR_PARAM,

    SOUND_INFO_REFERENCE_TO_EXTRA_USER_PARAM_TABLE = 0x1e,
    SOUND_INFO_USER_PARAM = 0x1f
};

enum WaveSoundInfoBitFlag
{
    WAVE_SOUND_INFO_PRIORITY = 0x00
};

enum SequenceSoundInfoBitFlag
{
    SEQ_SOUND_INFO_START_OFFSET = 0x00,
    SEQ_SOUND_INFO_PRIORITY
};

enum BankInfoBitFlag
{
    BANK_INFO_STRING_ID = 0x00
};

enum PlayerInfoBitFlag
{
    PLAYER_INFO_STRING_ID = 0x00,
    PLAYER_INFO_HEAP_SIZE
};

enum SoundGroupInfoBitFlag
{
    SOUND_GROUP_INFO_STRING_ID = 0x00
};

enum GroupInfoBitFlag
{
    GROUP_INFO_STRING_ID = 0x00
};

enum WaveArchiveInfoBitFlag
{
    WAVE_ARCHIVE_INFO_STRING_ID = 0x00,
    WAVE_ARCHIVE_INFO_WAVE_COUNT
};

}

SoundArchiveFile::FileLocationType SoundArchiveFile::FileInfo::GetFileLocationType() const
{
    switch(toFileLocation.typeId)
    {
    case 8716:
        return FILE_LOCATION_TYPE_INTERNAL;
    case 8717:
        return FILE_LOCATION_TYPE_EXTERNAL;
    case 0:
        return FILE_LOCATION_TYPE_NONE;
    default:
        NW_ASSERTMSG(false, "invalid file location type");
        return FILE_LOCATION_TYPE_NONE;
    }
}

} // namespace internal
} // namespace snd
} // namespace nw

