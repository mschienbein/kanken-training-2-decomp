struct ArchiveIndex;
struct ArchiveHandle {
    unsigned reserved00;
    struct ArchiveIndex *index;
};

extern unsigned func_00140a08(struct ArchiveIndex *index, unsigned resource_id);

unsigned func_0018d514(struct ArchiveHandle *handle, unsigned resource_id)
{
    return func_00140a08(handle->index, resource_id);
}
