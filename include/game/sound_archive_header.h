#ifndef GAME_SOUND_ARCHIVE_HEADER_H
#define GAME_SOUND_ARCHIVE_HEADER_H
#include "game/archive.h"
/* [INFERENCE] Fixed three-block CSAR header used by the bounded copy. */
struct SoundArchiveHeader {
    unsigned signature;
    unsigned short byte_order, header_size;
    unsigned version, file_size;
    unsigned short block_count, reserved12;
    struct ArchiveBlockReference blocks[3];
};
#ifdef __cplusplus
extern "C" {
#endif
void func_00147054(struct SoundArchiveHeader *destination,
                   const struct SoundArchiveHeader *source);
#ifdef __cplusplus
}
#endif
#endif
