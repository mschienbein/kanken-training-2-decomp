struct MemoryFileReaderW9J {
    unsigned char reserved00[0x3c];
    const void *data;
};
struct MemoryFileHeaderW9J {
    unsigned char reserved00[8];
};

void func_001470bc(struct MemoryFileReaderW9J *reader,
                   const struct MemoryFileHeaderW9J *file)
{
    reader->data = file + 1;
}
