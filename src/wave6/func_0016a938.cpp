#include "game/archive.h"

static __forceinline const void *tryLookup(SoundLookupProvider *provider, unsigned int sound_id)
{
    if (provider->enabled)
        return provider->vtable->lookup(provider, sound_id);
    return 0;
}

const void *func_0016a938(const SoundArchivePlayerView *player, unsigned int sound_id)
{
    if (player->provider != 0) {
        const void *result = tryLookup(player->provider, sound_id);
        if (result != 0)
            return result;
    }
    return func_001f2ac8(player->archive, sound_id);
}
