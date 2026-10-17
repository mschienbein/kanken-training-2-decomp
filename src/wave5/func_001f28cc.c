#include "game/archive.h"

struct ResourceData {
    unsigned char flags[4];
    unsigned reserved04;
    unsigned range_start;
    unsigned range_end;
    unsigned parameters[8];
    struct WordPair first_reference;
    unsigned short kind;
    unsigned short reserved3a;
    struct WordPair second_reference;
    unsigned extended_start;
    unsigned extended_end;
};

int func_001f28cc(const struct ResourceBlock *block, struct ResourceData *output)
{
    const struct ResourceData *source = func_0017ded4(block->reference);
    /* ORIGINAL BUG: unsupported reference types yield null, but the original
     * still reads the resource fields through that null source. */
    unsigned version;
    output->flags[0] = source->flags[0];
    output->flags[1] = source->flags[1];
    output->flags[2] = source->flags[2];
    output->flags[3] = source->flags[3];
    output->reserved04 = source->reserved04;
    output->range_start = source->range_start;
    output->range_end = source->range_end;
    output->parameters[0] = source->parameters[0];
    output->parameters[1] = source->parameters[1];
    output->parameters[2] = source->parameters[2];
    output->parameters[3] = source->parameters[3];
    output->parameters[4] = source->parameters[4];
    output->parameters[5] = source->parameters[5];
    output->parameters[6] = source->parameters[6];
    output->parameters[7] = source->parameters[7];
    output->first_reference = source->first_reference;
    output->kind = source->kind;
    output->second_reference = source->second_reference;
    version = block->file->version;
    output->extended_start = version >= 0x02030000 ? source->extended_start : source->range_start;
    output->extended_end = version >= 0x02030000 ? source->extended_end : source->range_end;
    return 1;
}
