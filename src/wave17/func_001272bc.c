#include "game/render_state.h"

void func_001272bc(unsigned index)
{
    struct PipelineStorage *storage = data_00242a14.storage;
    struct PipelineSnapshot *snapshot = &storage->snapshots[index];
    snapshot->value = storage->current;
}
