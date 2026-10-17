#ifndef GAME_MEMORY_FILE_H
#define GAME_MEMORY_FILE_H
struct MemoryFileReaderW9J { unsigned char reserved00[0x3c]; const void *data; };
struct MemoryFileHeaderW9J { unsigned char reserved00[8]; };
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001470bc(struct MemoryFileReaderW9J *reader, const struct MemoryFileHeaderW9J *file);
#ifdef __cplusplus
}
#endif
#endif
