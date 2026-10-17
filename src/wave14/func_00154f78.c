#include "game/display_animation.h"
#include "game/audio_voice.h"
#include "game/service_runtime.h"


void func_00154f78(struct HandleManager *router, int index,
                   const struct AudioChannelPacket *packet)
{
    func_00154f94(router->channels[index]->state, packet);
}
