#include "game/layout.h"

#include "game/command_group.h"

extern "C" void func_001c5950(struct AudioChannel *channel, unsigned mode)
{
    func_001c5958(channel->state, mode);
}
