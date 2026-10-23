#ifndef GAME_FILE_HANDLE_H
#define GAME_FILE_HANDLE_H

struct FileHandle;
struct Interface;

#include "game/result.h"
#include "ctr/ipc.h"
#include "game/file_session.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


struct InterfaceVTable {
    unsigned char reserved[0x30];
    void (*release)(Interface *object);
};
struct Interface {
    const InterfaceVTable *vtable;
};
struct FileHandle {
    Interface *object;
    unsigned reserved;
};

extern "C" {
unsigned func_001c4b74(FileSessionObject *owner, unsigned long long size);
unsigned func_001c4ef0(const unsigned *handle, unsigned *link_handle);
unsigned func_001c4b9c(FileSessionObject *owner, unsigned *link_handle);
unsigned func_001c4ec0(const unsigned *handle, unsigned priority);
unsigned func_001c4bbc(FileSessionObject *owner, unsigned priority);
unsigned func_001ef9d8(FileSessionObject *owner, unsigned long long *size);

Result func_0018fdd4(FileHandle handle);
}
#endif

#endif
