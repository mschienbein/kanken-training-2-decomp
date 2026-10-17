#include "game/prefetch_stream.h"
struct PrefetchReferenceTable;
struct PrefetchInfoBlock {
    unsigned reserved00[4];
    unsigned short channel_table_type;
    unsigned short reserved12;
    int channel_table_offset;
};
static inline const PrefetchReferenceTable *ResolveReference(const PrefetchInfoBlock *block)
{
    return reinterpret_cast<const PrefetchReferenceTable *>(reinterpret_cast<const char *>(block) + block->channel_table_offset);
}
extern "C" const PrefetchReferenceTable *func_0017e454(const PrefetchInfoBlock *block)
{
    if (block->channel_table_type != 0x101) return 0;
    return ResolveReference(block);
}
