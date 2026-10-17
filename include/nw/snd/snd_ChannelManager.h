/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_ChannelManager.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_CHANNEL_MANAGER_H_
#define NW_SND_CHANNEL_MANAGER_H_

#include "snd_Channel.h"
#include "snd_InstancePool.h"
#include "snd_FxBase.h"
#include "../ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {

class ChannelManager
{
public:
    static ChannelManager& GetInstance();

    size_t GetRequiredMemSize(int channelCount);
    void Initialize(void* mem, unsigned long memSize);
    void Finalize();
    Channel* Alloc();
    void Free(Channel* channel);

    void UpdateAllChannel();

private:
    typedef InstancePool<Channel> ChannelPool;
    typedef ut::LinkList< FxBase, offsetof(FxBase, m_Link) > FxList;
    typedef ut::LinkList< Channel, offsetof(Channel,m_Link)> ChannelList;

    ChannelManager();

    ChannelPool m_Pool;
    ChannelList m_ChannelList;
    bool m_IsInitialized;
    unsigned long m_ChannelCount;
    void* m_pMem;
    unsigned long m_MemSize;
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_CHANNEL_MANAGER_H_

