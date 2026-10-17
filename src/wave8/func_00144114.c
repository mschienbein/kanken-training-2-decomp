struct ArchiveResourceReference {
    unsigned short type;
    unsigned short reserved02;
    int offset;
};
struct ArchiveReferenceTable {
    unsigned count;
    struct ArchiveResourceReference entries[];
};
struct ArchiveInfoBlock {
    unsigned char reserved00[36];
    unsigned stream_table_offset;
    unsigned char reserved28[12];
    unsigned file_table_offset;
};

const struct ArchiveResourceReference *func_00144114(const struct ArchiveInfoBlock *info,
                                                    unsigned resource_id)
{
    const struct ArchiveReferenceTable *table = (const struct ArchiveReferenceTable *)
        ((const unsigned char *)info + info->file_table_offset);
    unsigned index = resource_id & 0x00ffffff;
    if (index >= table->count)
        return 0;
    return (const struct ArchiveResourceReference *)
        ((const unsigned char *)table + table->entries[index].offset);
}
