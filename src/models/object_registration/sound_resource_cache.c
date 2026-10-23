#include "game/sound_cache_registration.h"

/* [INFERENCE] The original registration identity was erased. Preserve its
 * concrete callback type without storing, invoking or adapting the destructor.
 * This source-only helper is uncredited and removed by the selected linker model. */
void register_sound_resource_cache_cleanup(struct SoundResourceCache *cache,
    void (*destroy)(struct SoundResourceCache *), void (*image_base)(void))
{
}
