#include <string.h>

struct W19aByteWriter {
    unsigned char *base;
    unsigned offset;
};

void func_0012238c(struct W19aByteWriter *writer, unsigned alignment)
{
    unsigned old_offset = writer->offset;
    unsigned mask = alignment - 1;
    unsigned aligned = (old_offset + mask) & ~mask;
    memset(writer->base + old_offset, 0, aligned - old_offset);
    writer->offset = aligned;
}
