#include "game/object_manager.h"

struct SoundArchiveFile;
struct SoundArchiveAccess {
    unsigned reserved00;
    const struct SoundArchiveFile *file;
    unsigned char reserved08[0x10c - 8];
    unsigned int block_offset;
    struct StreamMutex mutex;
};

extern unsigned int func_0014a31c(const struct SoundArchiveFile *file);

void func_001469a8(struct SoundArchiveAccess *access, const struct SoundArchiveFile *file)
{
    access->file = file;
    access->block_offset = func_0014a31c(file);
    func_00195ec4(&access->mutex);
}
