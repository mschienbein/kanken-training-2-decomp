#include "game/audio_voice.h"
void func_0017da08(struct AudioVoiceGroup *table, int index,
                   struct RecordState *record, int remember)
{
    struct AudioChannel *channel = table->channels[index];
    func_00156398(channel, record);
    if (remember)
        table->record = record;
}
