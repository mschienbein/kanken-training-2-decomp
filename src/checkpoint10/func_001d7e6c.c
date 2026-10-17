#include "game/audio_voice.h"
#include "game/object_manager.h"

struct SoundVoiceGroup;
struct SoundOperationEntry {
    unsigned char reserved00[8];
    struct SoundVoiceGroup *group;
    unsigned char reserved0c[0x3c8 - 0x0c];
};
struct SoundOperation {
    unsigned char reserved00[0x0d];
    unsigned char registered;
    unsigned char reserved0e[0x44 - 0x0e];
    struct RegistryEntry entry;
    unsigned char reserved50[0x10];
    struct RegistryEntry *control;
    unsigned char reserved64[0x220 - 0x64];
    unsigned count;
    struct SoundOperationEntry entries[];
};
extern void func_001d6678(struct RegistryEntry *entry);


void func_001d7e6c(struct SoundOperation *operation)
{
    unsigned i;
    func_001d6678(operation->control);
    for (i = 0; i < operation->count; ++i) {
        if (operation->entries[i].group)
            func_001835b8(operation->entries[i].group);
    }
    if (operation->registered) {
        func_0017e634(func_00185d1c(), &operation->entry);
        operation->registered = 0;
    }
}
