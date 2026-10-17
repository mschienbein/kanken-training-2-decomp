#ifndef GAME_FILE_HANDLE_H
#define GAME_FILE_HANDLE_H

struct FileHandleOwner;
struct FileHandle;
struct Interface;

#include "game/result.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


struct FileHandleOwner {
    unsigned reserved00;
    unsigned handle;
};
struct FileHandle {
    Interface *object;
    unsigned reserved;
};

extern "C" {
unsigned func_0016bd7c(FileHandleOwner *owner);
unsigned func_001c5044(const unsigned *handle, unsigned long long size);
unsigned func_001c4b74(FileHandleOwner *owner, unsigned long long size);
unsigned func_001c4ef0(const unsigned *handle, unsigned *link_handle);
unsigned func_001c4b9c(FileHandleOwner *owner, unsigned *link_handle);
unsigned func_001c4ec0(const unsigned *handle, unsigned priority);
unsigned func_001c4bbc(FileHandleOwner *owner, unsigned priority);
unsigned func_001c500c(const unsigned *handle, unsigned long long *size);
unsigned func_001ef9d8(FileHandleOwner *owner, unsigned long long *size);

Result func_0018fdd4(FileHandle handle);
}
#endif

#endif
