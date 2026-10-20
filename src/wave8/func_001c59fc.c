#include "game/layout.h"

void func_001c59fc(struct AudioChannel *channel, unsigned alignment)
{
    channel->alignment = alignment;
    func_001c5a08(channel->state, alignment);
}
