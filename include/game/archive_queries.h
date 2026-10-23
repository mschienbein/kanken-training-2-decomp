#ifndef GAME_ARCHIVE_QUERIES_H
#define GAME_ARCHIVE_QUERIES_H
#include "game/archive.h"
#ifdef __cplusplus
extern "C" {
#endif
unsigned func_00139174(const struct SoundArchiveInfo *info, unsigned identifier);
unsigned func_001f1ef4(const struct ArchiveFile *file);
void func_001d47c8(struct ResourceBlock *reader, const struct ArchiveFile *file);
#ifdef __cplusplus
}
#endif
#endif
