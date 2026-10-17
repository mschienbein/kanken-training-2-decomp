/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_Arc.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located methods and required public private types; algorithms unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#define NW_RELEASE
#include <nw/lyt/lyt_Arc.h>

namespace nw { namespace lyt {
struct FSTArcEntry
{
    u32    isDirAndStringOff;
    u32    parentOrPosition;
    u32    nextEntryOrLength;
};

#define entryIsDir(fstStart, i)     \
    (((fstStart[i].isDirAndStringOff & 0xff000000) == 0)? false:true)
#define stringOff(fstStart, i)      \
        (fstStart[i].isDirAndStringOff & 0x00ffffff )
#define parentDir(fstStart, i)       \
        (fstStart[i].parentOrPosition )
#define nextDir(fstStart, i)        \
        (fstStart[i].nextEntryOrLength )
#define filePosition(fstStart, i)       \
        (fstStart[i].parentOrPosition )
#define fileLength(fstStart, i)         \
        (fstStart[i].nextEntryOrLength )

bool ARCInitHandle(void* arcStart, ARCHandle* handle)
{
    FSTArcEntry*           FSTEntries;
    ARCHeader*          arcHeader;

    arcHeader = (ARCHeader*)arcStart;
    
    handle->archiveStartAddr = arcStart;
    handle->FSTStart = FSTEntries = reinterpret_cast<FSTArcEntry*>((u32)arcStart + arcHeader->fstStart);
    handle->fileStart = (void*)((u32)arcStart + arcHeader->fileStart);

    handle->entryNum = nextDir(FSTEntries, 0);
    handle->FSTStringStart = reinterpret_cast<wchar_t*>(&(FSTEntries[handle->entryNum]));
    handle->FSTLength = (u32)arcHeader->fstSize;
    handle->currDir = 0;

    return true;
}

bool ARCFastOpen(ARCHandle* handle, s32 entrynum, ARCFileInfo* af)
{
    FSTArcEntry*           FSTEntries;

    FSTEntries = (FSTArcEntry*)handle->FSTStart;


    if ((entrynum < 0) || (entrynum >= static_cast<s32>(handle->entryNum)) || entryIsDir(FSTEntries, entrynum))
    {
        return false;
    }

    af->handle = handle;
    af->startOffset = filePosition(FSTEntries, entrynum);
    af->length = fileLength(FSTEntries, entrynum);

    return true;
}

} } 
