#include "game/layout.h"

#include "game/layout_draw.h"

extern "C" void func_001c5970(struct AudioChannel *channel, unsigned direction)
{
    func_001c5978(channel->state, direction);
}
