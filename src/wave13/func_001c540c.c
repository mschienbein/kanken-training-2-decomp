#include "game/command_group.h"
#include "game/archive.h"

void func_001c540c(struct ArchiveReader *reader)
{
    reader->file = 0;
    reader->info = 0;
    reader->cache = 0;
    reader->initialized = 0;
}
