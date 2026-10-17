struct WordRingWriter {
    unsigned *data;
    int capacity;
    unsigned reserved08;
    int position;
};

#include <string.h>

void func_00154378(struct WordRingWriter *writer, const unsigned *source, int count)
{
    memcpy(writer->data + writer->position, source, count * sizeof(*source));
    writer->position += count;
    if (writer->position >= writer->capacity)
        writer->position = 0;
}
