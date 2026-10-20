#include <preinclude.h>
#include <nw/io/io_FileStream.h>

namespace nw {
namespace snd {
namespace internal {

/* [INFERENCE] Class identity and member names. The vtable at data_0022471c
 * (slots Read = func_001d3d44, Close = func_001d3f08, Seek = func_001d3e90)
 * belongs to a FileStream that wraps another stream (+0x14): Close forwards
 * to it, and Seek works inside a cached window (+0x18 buffer, +0x1c size,
 * +0x20 window offset, +0x24 position) before falling back to the wrapped
 * stream. Only the constructor is defined here; Close is the first non-inline
 * virtual, so the vtable is emitted elsewhere (bound as _ZTV in names). */
class BufferedFileStream : public io::FileStream
{
public:
    BufferedFileStream(io::FileStream* stream);
    virtual void Close();

private:
    io::FileStream* m_pStream;
    void* m_pBuffer;
    u32 m_BufferSize;
    s32 m_BufferOffset;
    s32 m_Position;
};

BufferedFileStream::BufferedFileStream(io::FileStream* stream):
    m_pStream(stream),
    m_pBuffer(NULL),
    m_BufferSize(0),
    m_BufferOffset(-1),
    m_Position(-1)
{
    m_Available = true;
}

} // namespace internal
} // namespace snd
} // namespace nw
