/* [INFERENCE] Command/data role names are partial views.  The void update
 * contract follows the native preparation wrappers; the observed direct caller
 * discards the queue helper's incidental result. */
#include "game/sound_runtime.h"
#include "game/stream_manager.h"

struct SequenceStartOptions {
    unsigned char paused;
    unsigned char reserved01[3];
    unsigned position;
    unsigned offset;
    unsigned limit;
};

struct SequenceDataBinding {
    unsigned reserved00[5];
    const void *data;
};
struct SequenceCommandSound : SequenceSound {
    unsigned char reserved230[0x20e4];
    SequenceDataBinding binding;
};
struct SequenceStartEvent : StreamEvent {
    unsigned reserved14;
    SequenceStartOptions options;
    SequenceDataBinding *binding;
    StreamEvent *initialize(SoundPlaybackState *player, SequenceDataBinding *new_binding,
                            const SequenceStartOptions &new_options)
    {
        opcode = 0x28;
        object = player;
        binding = new_binding;
        options = new_options;
        return this;
    }
};

template <class Command>
static Command *allocate_command(StreamManager *manager)
{
    Command *command = static_cast<Command *>(func_0019147c(manager, (sizeof(Command) + 3) / 4));
    command->start = manager->write_index;
    return command;
}

/* INTEGRATOR: move the entry prototype and command types to include/game/sound_commands.h. */
extern "C" void func_001d31cc(SequenceCommandSound *sound, const void *data,
                                 const SequenceStartOptions *options);
void func_001d31cc(SequenceCommandSound *sound, const void *data,
                       const SequenceStartOptions *options)
{
    sound->binding.data = data;
    StreamManager *manager = func_001916e8();
    SequenceStartEvent *event = allocate_command<SequenceStartEvent>(manager);
    func_00191458(manager, event->initialize(&sound->playback, &sound->binding, *options));
}
