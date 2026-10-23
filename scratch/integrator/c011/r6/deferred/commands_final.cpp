#include "game/stream_manager.h"

/* [INFERENCE] Command variant spelling; the base fields use canonical layout. */
struct ChannelCommand : StreamEvent {
    unsigned reserved14;
    unsigned enabled;
    unsigned char &channel_storage()
    {
        return *reinterpret_cast<unsigned char *>(&object);
    }
    StreamEvent *initialize(unsigned channel_value, unsigned enabled_value)
    {
        opcode = 0x39;
        channel_storage() = channel_value;
        enabled = enabled_value;
        return this;
    }
};

static ChannelCommand *allocate_channel_command(StreamManager *manager)
{
    ChannelCommand *command = static_cast<ChannelCommand *>(func_0019147c(manager, 7));
    command->start = manager->write_index;
    return command;
}

static void dispatch_channel(StreamManager *manager, unsigned channel, unsigned enabled)
{
    ChannelCommand *command = allocate_channel_command(manager);
    func_00191458(manager, command->initialize(channel, enabled));
}

void func_001911f0(unsigned channel, unsigned enabled)
{
    StreamManager *manager = func_001916e8();
    dispatch_channel(manager, channel, enabled);
    unsigned serial = func_001912b0(manager, 1);
    func_0019124c(manager, serial);
}
