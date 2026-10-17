struct PrefetchBlockReference {
    unsigned short type;
    unsigned short reserved02;
    int offset;
    unsigned size;
};
struct PrefetchFileHeader {
    unsigned signature;
    unsigned short byte_order;
    unsigned short header_size;
    unsigned version;
    unsigned file_size;
    unsigned short block_count;
    unsigned short reserved12;
    struct PrefetchBlockReference blocks[];
};
struct PrefetchInfoBlock;
struct PrefetchDataBlock;
struct PrefetchFileBlock {
    unsigned signature;
    unsigned size;
};
struct PrefetchFileReader {
    const struct PrefetchFileHeader *file;
    const struct PrefetchInfoBlock *info;
    const struct PrefetchDataBlock *data;
};
extern const struct PrefetchFileBlock *func_00153ebc(const struct PrefetchFileHeader *file);
extern const struct PrefetchFileBlock *func_00153f28(const struct PrefetchFileHeader *file);

void func_001528dc(struct PrefetchFileReader *reader, const struct PrefetchFileHeader *file)
{
    const struct PrefetchFileBlock *info;
    const struct PrefetchFileBlock *data;
    if (file->signature != 0x50545343 || (file->version - 0x10000u) > 0x10200u)
        return;
    reader->file = file;
    info = func_00153ebc(file);
    /* ORIGINAL BUG: a missing block is dereferenced before its signature is checked. */
    if (info->signature != 0x4f464e49)
        return;
    data = func_00153f28(reader->file);
    if (data->signature != 0x54414450)
        return;
    reader->info = (const struct PrefetchInfoBlock *)(info + 1);
    reader->data = (const struct PrefetchDataBlock *)(data + 1);
}
