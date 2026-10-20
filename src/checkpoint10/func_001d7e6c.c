#include "game/audio_voice.h"
#include "game/audio_tasks.h"
#include "game/object_manager.h"



void func_001d7e6c(struct SoundOperation *operation)
{
    unsigned i;
    func_001d6678(operation->task_id);
    for (i = 0; i < operation->count; ++i) {
        if (operation->entries[i].group)
            func_001835b8(operation->entries[i].group);
    }
    if (operation->registered) {
        func_0017e634(func_00185d1c(), &operation->entry);
        operation->registered = 0;
    }
}
