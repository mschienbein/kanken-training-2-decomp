/* [INFERENCE] Command/data role names are partial views.  The void update
 * contract follows the native preparation wrappers; the observed direct caller
 * discards the queue helper's incidental result. */
#include "game/sound_runtime.h"
#include "game/stream_manager.h"



template <class Command>
static Command *allocate_command(StreamManager *manager)
{
    Command *command = static_cast<Command *>(func_0019147c(manager, (sizeof(Command) + 3) / 4));
    command->start = manager->write_index;
    return command;
}

void func_001d31cc(SequenceCommandSound *sound, const void *data,
                       const SequenceStartOptions *options)
{
    sound->binding.data = data;
    StreamManager *manager = func_001916e8();
    SequenceStartEvent *event = allocate_command<SequenceStartEvent>(manager);
    func_00191458(manager, event->initialize(&sound->playback, &sound->binding, *options));
}
