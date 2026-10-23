#include "game/archive.h"
#include "game/sound_runtime.h"
#include "game/object_manager.h"

struct W24eSoundLayer {
    unsigned reserved00;
    const struct SoundArchivePlayerView *player;
};
struct W24eSoundChannel {
    const void *vtable;
    const struct W24eSoundLayer *layer;
};
struct W24eBankRecord { unsigned fields[4]; };
struct W24eNoteRecord { unsigned fields[3]; };
struct W24eInstrumentSource {
    unsigned char reserved00[0xf4];
    struct W24eBankRecord banks[4];
    struct W24eNoteRecord notes[];
};
struct W24eVoiceParameters;
extern struct SoundInstance *func_001f2f60(struct SoundHandle *handle,
    const struct W24eNoteRecord *note, const struct W24eBankRecord *bank,
    const struct W24eVoiceParameters *parameters);

struct SoundInstance *func_001d1c10(const struct W24eSoundChannel *channel,
    const struct W24eInstrumentSource *source, int index,
    const struct W24eVoiceParameters *parameters)
{
    if (channel->layer->player && func_0018d850(channel->layer->player)) {
        const struct W24eNoteRecord *note = &source->notes[index];
        struct SoundHandle handle;
        struct SoundInstance *sound;
        struct ObjectManager *manager;
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00125b54(&handle);
        sound = func_001f2f60(&handle, note, &source->banks[index], parameters);
        manager = func_00185d1c();
        ++manager->voice_requests;
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00125b54(&handle);
        return sound;
    }
    return 0;
}
