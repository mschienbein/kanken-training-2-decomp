#include "game/render_state.h"

extern void func_0019cecc(void *destination, unsigned size);
extern struct PipelineConfiguration *func_0018c6d4(void);

int func_0011d260(struct PipelineStorage *storage)
{
    struct PipelineConfiguration *configuration;
    int result;
    data_00242a14.storage = storage;
    func_0019cecc(storage, PIPELINE_STORAGE_INITIALIZED_BYTES);
    configuration = func_0018c6d4();
    data_00242a14.storage->configuration = configuration;
    if (configuration) {
        data_00242a14.ready = 1;
        result = 0;
    } else {
        if (data_002429f8)
            data_002429f8(0x10000, 0x100, 0, data_00242a14.storage);
        data_00242a14.storage = 0;
        result = -1;
    }
    return result;
}
