/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_StreamBufferPool.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: public header unchanged except standalone relative include paths
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_STREAM_BUFFER_POOL_H_
#define NW_SND_STREAM_BUFFER_POOL_H_

#include "snd_Global.h"

namespace nw { 
namespace snd { 
namespace internal { 
namespace driver {

class StreamBufferPool
{
public:
    void Initialize(void* pBuffer, size_t size, int blockCount);
    void Finalize();

    void* Alloc();
    void Free(void* pPtr);

    size_t GetBlockSize() const { return m_BlockSize; }

private:
    static const int BLOCK_MAX = 32;
    static const int BIT_PER_BYTE = 8;

    void* m_pBuffer;
    size_t m_BufferSize;
    size_t m_BlockSize;
    int m_BlockCount;
    int m_AllocCount;
    u8 m_AllocFlags[BLOCK_MAX / BIT_PER_BYTE];
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_STREAM_BUFFER_POOL_H_

