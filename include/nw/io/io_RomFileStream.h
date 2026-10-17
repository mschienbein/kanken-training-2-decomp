/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/io/io_RomFileStream.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <nn/fs.h>
#include "io_FileStream.h"

namespace nw {
namespace io {

class RomFileStream : public FileStream
{
public:
    NW_UT_RUNTIME_TYPEINFO;

    RomFileStream(const char* path);
    RomFileStream(FileReader* openedFileReader, bool closeEnable = true );

    bool Open(const char* path);
    bool Open(FileReader* openedFileReader, bool closeEnable = true );

    virtual ~RomFileStream();
    virtual void Close();
    virtual s32 Read(void* buf, u32 length);
    virtual void Seek(s32 offset, u32 origin);
    virtual u32 GetSize() const { return this->m_FilePosition.GetFileSize(); }
    virtual u32 Tell() const { return this->m_FilePosition.Tell(); }
    virtual bool CanSeek() const { return true; }
    virtual bool CanCancel() const { return false; }
    virtual bool CanRead() const { return true; }
    virtual bool CanWrite() const { return false; }
    virtual bool CanAsync() const { return false; }

private:
    void Initialize();

    FilePosition m_FilePosition;
    FileReader m_FileReader;
    FileReader* m_pOpenedFileReader;
    bool m_IsAvailable;
    bool m_CloseOnDestroyFlag;
    bool m_CloseEnableFlag;
};

}
}

