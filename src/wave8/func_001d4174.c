struct ArchiveHandle;

struct SoundArchiveRequestState {
    unsigned pending;
    struct ArchiveHandle *archive;
};

void func_001d4174(struct SoundArchiveRequestState *state,
                   struct ArchiveHandle *archive)
{
    state->archive = archive;
}
