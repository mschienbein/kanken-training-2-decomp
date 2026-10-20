#include "game/audio_voice.h"
#include "game/functions.h"

struct AudioChannel;

#include <stddef.h>


extern "C" int func_0015477c(nw::snd::internal::driver::VoiceManager *pool, int priority)
{
    nw::snd::internal::driver::Voice *group;
    int released;
    nw::snd::internal::driver::Voice::VoiceCallback callback;
    void *context;
    if (!pool->m_PrioVoiceList.size())
        return 0;
    group = &pool->m_PrioVoiceList.front();
    if (group->priority > priority)
        return 0;
    released = group->count;
    callback = group->callback;
    context = group->context;
    func_001835b8(group);
    func_00183550(group);
    if (callback)
        callback(group, 2, context);
    return released;
}
