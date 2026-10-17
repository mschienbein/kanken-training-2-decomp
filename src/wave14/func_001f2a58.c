#include "game/event_route.h"
#include <stdbool.h>
#include "game/archive.h"


bool func_001f2a58(const struct BinaryVersionReader *reader)
{
    return reader->file->version <= 0x02010000;
}
