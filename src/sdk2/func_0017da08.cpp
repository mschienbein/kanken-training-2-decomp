#include "game/audio_voice.h"
extern "C" void func_0017da08(nw::snd::internal::driver::Voice *table, int index,
                   struct RecordState *record, int remember)
{
    struct AudioChannel *channel = table->channels[index];
    func_00156398(channel, record);
    if (remember)
        table->record = record;
}
