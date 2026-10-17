#include "game/display_animation.h"
#include "game/audio_voice.h"


void func_00154f78(struct AudioPacketRouter *router, int index,
                   const struct AudioChannelPacket *packet)
{
    func_00154f94(router->channels[index]->state, packet);
}
