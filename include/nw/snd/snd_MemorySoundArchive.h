/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_MemorySoundArchive.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: relative includes; declare the implicit MemoryFileStream destructor out of line, leaving its original VFE-stripped vtable in the image.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_MEMORY_SOUND_ARCHIVE_H
#define NW_SND_MEMORY_SOUND_ARCHIVE_H

#include "snd_SoundArchiveFileReader.h"
#include "../io/io_FileStream.h"

namespace nw {
namespace io {
    class FileStream;
}
namespace snd {

class MemorySoundArchive : public SoundArchive 
{
private:
    class MemoryFileStream;

public:
    MemorySoundArchive();
    virtual ~MemorySoundArchive();

    virtual size_t detail_GetRequiredStreamBufferSize() const;
    virtual const void* detail_GetFileAddress(FileId fileId) const;

    bool Initialize(const void* soundArchiveData);
    void Finalize();

protected:
    virtual io::FileStream* OpenStream(void* buffer, int size, u32 begin,
        u32 length) const;

    virtual io::FileStream* OpenExtStream(void* buffer, int size,
        const char* extFilePath, u32 begin, u32 length) const;

private:
    const void* m_pData;
    internal::SoundArchiveFileReader m_FileReader;
};

class MemorySoundArchive::MemoryFileStream : public io::FileStream
{
public:
    MemoryFileStream(const void* buffer, u32 size);
    virtual ~MemoryFileStream();

    virtual bool CanSeek() const { return true; }
    virtual bool CanCancel() const { return true; }
    virtual bool CanAsync() const { return false; }
    virtual bool CanRead() const { return true; }
    virtual bool CanWrite() const { return false; }

    virtual void Close();
    virtual s32  Read(void* buf, u32 length);
    virtual void Seek(s32 offset, u32 origim);
    virtual u32  Tell() const { return m_Position; }
    virtual u32  GetSize() const { return m_Size; }

private:
    const void* m_pBuffer;
    u32 m_Size;
    u32 m_Position;
};

} // namespace snd
} // namespace nw

#endif // NW_SND_MEMORY_SOUND_ARCHIVE_H

