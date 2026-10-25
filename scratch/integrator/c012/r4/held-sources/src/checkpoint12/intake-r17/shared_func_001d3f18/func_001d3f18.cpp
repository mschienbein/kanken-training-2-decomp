#include "game/buffered_file_stream.h"

namespace nw { namespace snd { namespace internal {
BufferedFileStream::BufferedFileStream(io::FileStream* stream):
    m_pStream(stream),
    m_pBuffer(NULL),
    m_BufferSize(0),
    m_BufferOffset(-1),
    m_Position(-1)
{
    m_Available = true;
}

} } }
