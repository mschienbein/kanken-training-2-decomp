struct WordRingWriter {
    unsigned *data;
    int capacity;
    unsigned reserved08;
    int position;
};

#include "game/runtime.h"

void func_00154378(struct WordRingWriter *writer, const unsigned *source, int count)
{
    /* The original calls the general copy entry even for word-aligned buffers. */
    func_001a000c(writer->data + writer->position, source, count * sizeof(*source));
    writer->position += count;
    if (writer->position >= writer->capacity)
        writer->position = 0;
}
