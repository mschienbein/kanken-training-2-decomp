#include "game/bss_0049ecf0.h"

extern unsigned char *data_00242fc8;
extern void func_00192720(void *destination, const void *source, unsigned size);
extern void func_00195f7c(void);
extern void func_0012fb38(void);
extern void func_0012fb48(void);
extern void func_00190878(void);

void func_00195254(const void *source, unsigned size, int immediate)
{
    struct CommandQueue *queue = bss_0049ecf0.writing;
    struct Command *command;
    queue->offset = data_00242fc8 - queue->base;
    if (immediate) {
        func_00192720(data_00242fc8, source, size);
        data_00242fc8 += size & ~3u;
    } else {
        if (queue->committed_offset != queue->offset)
            func_00195f7c();
        if (queue->committed_offset == queue->offset) {
            command = &queue->commands[queue->count];
            command->kind = 1;
            command->size = size;
            command->source = source;
            command->flags |= 2;
            func_0012fb38();
            ++queue->count;
            if (bss_0049ecf0.executing == queue &&
                bss_0049ecf0.enabled && !bss_0049ecf0.pending) {
                bss_0049ecf0.pending = 1;
                func_00190878();
            }
            func_0012fb48();
        }
    }
}
