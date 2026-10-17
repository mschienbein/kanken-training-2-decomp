/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/ut/ut_BinaryFileFormat.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select the located block iterator and retain its endian-test helper inline; algorithm unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#define NW_RELEASE
#include <nw/types.h>
#include <nw/assert.h>
#include <nw/ut/ut_BinaryFileFormat.h>

namespace nw { namespace ut {
inline bool IsReverseEndianBinaryFile(const BinaryFileHeader* pFileHeader)
{
    NW_POINTER_ASSERT(pFileHeader);
    return (pFileHeader->byteOrder != BYTE_ORDER_MARK);
}

BinaryBlockHeader*  GetNextBinaryBlockHeader(BinaryFileHeader* pFileHeader,BinaryBlockHeader* pBlockHeader)
{
    NW_POINTER_ASSERT(pFileHeader);
    
    void* ptr;
    if (!IsReverseEndianBinaryFile(pFileHeader))
    {
        if (pBlockHeader == NULL)
        {
            if (pFileHeader->dataBlocks == 0) return NULL;
            ptr = AddOffsetToPtr(pFileHeader, pFileHeader->headerSize);
        }
        else
        {
            ptr = AddOffsetToPtr(pBlockHeader, pBlockHeader->size);
        }
        
        if (ptr >= AddOffsetToPtr(pFileHeader, pFileHeader->fileSize))
        {
            return NULL;
        }
    }
    else
    {
        if (pBlockHeader == NULL)
        {
            if (pFileHeader->dataBlocks == 0 ) return NULL;
            ptr = AddOffsetToPtr( pFileHeader, ReverseEndian(pFileHeader->headerSize));
        }
        else
        {
            ptr = AddOffsetToPtr(pBlockHeader, ReverseEndian(pBlockHeader->size));
        }
        
        if (ptr >= AddOffsetToPtr(pFileHeader, ReverseEndian(pFileHeader->fileSize)))
        {
            return NULL;
        }
    }
    
    return reinterpret_cast<BinaryBlockHeader*>(ptr);
}

} } 
