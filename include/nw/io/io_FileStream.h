/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/io/io_FileStream.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "io_IOStream.h"

namespace nw   {
namespace io     {

enum
{
    FILE_STREAM_SEEK_BEGIN,
    FILE_STREAM_SEEK_CURRENT,
    FILE_STREAM_SEEK_END
};

class FileStream : public IOStream
{
public:
    NW_UT_RUNTIME_TYPEINFO;

    virtual u32 GetSize() const = 0;
    virtual void Seek(s32 offset, u32 origin);
    virtual void Cancel(void);
    virtual bool CancelAsync(IOStreamCallback callback, void* arg);
    
    virtual bool CanSeek (void) const = 0;
    virtual bool CanCancel(void) const = 0;
    virtual u32 Tell()  const = 0;
    bool IsEof() const { return Tell() >= GetSize();  }

protected:
    FileStream(): 
        IOStream() 
    {
    }
        
    class FilePosition
    {
    public:
        FilePosition(): 
            m_FileSize(0), 
            m_Position(0) 
        {
        }
        void SetFileSize(u32 fileSize) { m_FileSize = fileSize; }
        u32 GetFileSize() const { return m_FileSize; }
        
        u32 Skip(s32 offset);
        u32 Append(s32 offset);
        void Seek(s32 offset, u32 origin);
        u32 Tell() const { return m_Position; }
    private:
        u32 m_FileSize;
        u32 m_Position;
    };
};


}
}

