#include "game/bss_0049ecf0.h"

struct CommandPatch;
struct CommandQueueDetail {
    unsigned id;
    unsigned char *base;
    unsigned capacity_bytes;
    unsigned offset;
    unsigned command_capacity;
    unsigned committed_offset;
    struct Command *commands;
    unsigned command_bytes;
    int count;
    int completed_count;
    int cursor;
    unsigned capacity;
    unsigned captured_size;
    struct CommandPatch *patch;
    unsigned reserved38;
    unsigned char complete;
    unsigned char flush;
};
extern unsigned char *data_00242fc8;
extern unsigned func_00122890(unsigned register_address, void *destination, unsigned size);

void func_0019b574(unsigned parameter, unsigned *value)
{
    struct CommandQueueDetail *queue = (struct CommandQueueDetail *)bss_0049ecf0.writing;
    switch (parameter) {
    case 0x200:
        *value = queue->capacity;
        break;
    case 0x201:
        *value = bss_0049ecf0.executing == (struct CommandQueue *)queue && bss_0049ecf0.pending;
        break;
    case 0x202:
        queue->offset = data_00242fc8 - queue->base;
        *value = queue->offset;
        break;
    case 0x203:
        *value = queue->count;
        break;
    case 0x204:
        *value = queue->capacity_bytes;
        break;
    case 0x205:
        *value = queue->command_bytes;
        break;
    case 0x206:
        *value = (unsigned)queue->base;
        break;
    case 0x207:
        *value = queue ? queue->id : 0;
        break;
    case 0x208:
        *value = (unsigned)data_00242fc8;
        break;
    case 0x209:
        *value = queue->command_capacity;
        break;
    case 0x20a:
        *value = queue->cursor;
        break;
    case 0x20b:
        *value = (unsigned)queue->commands;
        break;
    case 0x20c:
        if (queue->count > queue->cursor) {
            switch (queue->commands[queue->cursor].kind) {
            case 0: *value = 0x310; break;
            case 1: *value = 0x311; break;
            case 2: *value = 0x312; break;
            case 3: *value = 0x313; break;
            case 4: *value = 0x314; break;
            }
        }
        break;
    case 0x20d:
        if (queue->count > queue->cursor && queue->commands[queue->cursor].kind == 1) {
            *value = (unsigned)queue->commands[queue->cursor].source;
            value[1] = queue->commands[queue->cursor].size;
        }
        break;
    case 0x20e:
        func_00122890(0x400058, value, 4);
        break;
    }
}
