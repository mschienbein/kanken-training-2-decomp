#include "game/file_stream.h"
#include "game/result.h"



Result func_001c4af0(struct FileHandleObject *file, int priority)
{
    if (!file)
        return Result(0xe0e046bc);
    return file->set_priority(priority);
}
