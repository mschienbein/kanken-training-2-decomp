#include "game/archive.h"

/* Unobserved virtual slots describe layout only; their signatures are not recovered or invoked here. */

bool func_00138edc(const SoundArchive *archive, unsigned file_id, ArchiveFileLocation *out)
{
    bool result = func_00140e14(archive->reader, file_id, out, 0);
    if (result && archive->addon) {
        archive->addon->read_file_location(file_id, out, 0);
        return true;
    }
    return result;
}
