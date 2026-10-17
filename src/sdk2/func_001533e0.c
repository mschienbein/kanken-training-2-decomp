#include "game/prefetch_stream.h"
#include "game/runtime.h"
int func_001533e0(const struct PrefetchFileReader *reader, struct PrefetchChannelInfo *output,
                  unsigned index)
{
    const struct PrefetchChannelEntry *entry = &reader->data->entries[index];
    output->start = entry->start;
    output->size = entry->size;
    /* [INFERENCE] callee identity: inlined by armlink; this call preserves the mapped pointer. */
    output->data = func_00137150((void *)func_00153f90(entry));
    return 1;
}
