#ifndef GAME_RESOURCE_STREAM_H
#define GAME_RESOURCE_STREAM_H

#include "game/resource.h"
#include "game/runtime.h"

struct ResourceStream {
    unsigned resource_bits;
    ResourceSize position;
    ResourceSize size;
};

struct ResourceFile {
    const void *vtable;
    ResourceStream stream;
    Result result;
    unsigned char recoverable;
    unsigned char reserved1d[3];
};

extern "C" {
int func_00146dfc(ResourceFile *file, int offset, int origin);
int func_00146d98(ResourceFile *file, void *buffer, unsigned size, int *transferred);
Result func_00180944(ResourceStream *stream, long long offset, int origin);
Result func_0019d5e4(ResourceStream *stream, int *transferred, void *buffer, unsigned size);
Result func_0019d66c(ResourceStream *stream, ResourceSize *size);
}

#endif
