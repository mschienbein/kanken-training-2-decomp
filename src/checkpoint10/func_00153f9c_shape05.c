#include "game/prefetch_stream.h"
struct PrefetchChannelReference;
extern const struct PrefetchReferenceTable *func_0017e454(const struct PrefetchInfoBlock *block);
extern const struct PrefetchChannelReference *func_00154804(const struct PrefetchReferenceTable *table,
                                                           unsigned index);
extern const struct AdpcmChannelParameters *func_001547ec(const struct PrefetchChannelReference *reference);
extern void func_0017da60(struct AdpcmLoopState *output, const struct AdpcmLoopState *input);

int func_00153f9c(const struct PrefetchFileReader *reader, struct AdpcmChannelParameters *parameters,
                  struct AdpcmLoopState *loop, unsigned index)
{
    /* ORIGINAL BUG: outer references are used without checking. */
    struct AdpcmLoopState *loop_output = loop;
    struct AdpcmChannelParameters *output = parameters;
    const struct AdpcmChannelParameters *source = func_001547ec(func_00154804(func_0017e454(reader->info), index));
    if (source == 0) return 0;
    *output = *source;
    func_0017da60(loop_output, (const struct AdpcmLoopState *)(source + 1));
    return 1;
}
