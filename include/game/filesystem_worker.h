#ifndef GAME_FILESYSTEM_WORKER_H
#define GAME_FILESYSTEM_WORKER_H

#include "game/runtime.h"
#include "game/service_state.h"

/* [INFERENCE] Work-item names follow the worker's indexed 44-byte records. */
struct FilesystemWorkItem {
    int active;
    int kind;
    const char *path;
    void *allocation;
    unsigned reserved10[6];
    int reformat;
};

#ifdef __cplusplus
extern "C" {
#endif
extern const char *data_00242218;
unsigned func_00120e40(unsigned files, unsigned directories, unsigned char duplicate_data);
int func_00110f7c(const char *path, int read_only);
int func_00111570(struct FilesystemWorkItem *item);
int func_0011122c(struct FilesystemWorkItem *item);
unsigned func_001131f8(const char *path);
unsigned func_0011343c(const char *path);
/* Exact invoking thread-entry protocol; context is retained even when unused. */
void func_0010af8c(void *argument, void *context);
#ifdef __cplusplus
}
#endif
#endif
