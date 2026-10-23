#include <preinclude.h>
#include <nw/snd/snd_MemorySoundArchive.h>
#include <algorithm>
#include <cstring>

namespace nw {
namespace snd {

static const u8 *storage_data(const void *const& storage)
{
    return static_cast<const u8 *>(storage);
}

s32 MemorySoundArchive::MemoryFileStream::Read(void *buffer, u32 length)
{
    u32 remaining = m_Size - m_Position;
    u32 amount = remaining < length ? remaining : length;
    std::memcpy(buffer, storage_data(m_pBuffer) + m_Position, amount);
    m_Position += amount;
    return amount;
}

} // namespace snd
} // namespace nw
