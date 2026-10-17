#include "game/bss_0049ecf0.h"

struct CommandPatch {
    unsigned reserved00[2];
    unsigned pair_count;
    unsigned reserved0c[3];
};
struct ExtendedCommandQueue {
    struct CommandQueue queue;
    int cursor;
    unsigned capacity;
    unsigned captured_size;
    struct CommandPatch *patch;
    unsigned reserved38;
    unsigned char complete;
    unsigned char flush;
};
extern unsigned func_0013776c(void *buffer, unsigned size);

void func_0012ccd8(struct ExtendedCommandQueue *detail, struct Command *command)
{
    detail->patch->pair_count = ((unsigned)((const unsigned char *)command->source + command->size -
        (const unsigned char *)detail->patch) - sizeof(struct CommandPatch)) >> 3;
    func_0013776c((void *)command->source, command->size);
    command->flags |= 4;
    command->size = detail->captured_size;
    detail->patch = 0;
    detail->captured_size = 0;
}
