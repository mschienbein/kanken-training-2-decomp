struct PrefetchFileHeader;
struct PrefetchInfoBlock;
struct PrefetchDataBlock;
struct PrefetchFileReader {
    const struct PrefetchFileHeader *file;
    const struct PrefetchInfoBlock *info;
    const struct PrefetchDataBlock *data;
};

void func_00152964(struct PrefetchFileReader *reader)
{
    reader->file = 0;
    reader->info = 0;
    reader->data = 0;
}
