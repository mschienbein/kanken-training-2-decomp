#ifndef GAME_ARCHIVE_IPC_H
#define GAME_ARCHIVE_IPC_H

struct ArchiveInfo;
struct ArchiveOutputCommand;

#include "ctr/svc.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ArchiveOutputCommand {
    unsigned int header;
    unsigned int count;
    union { unsigned char mode; unsigned int value; } selector;
    unsigned int id_low;
    unsigned int id_high;
    unsigned int flags;
    unsigned int descriptor;
    struct ArchiveInfo *buffer;
};

#ifdef __cplusplus
extern "C" {
#endif
extern CtrHandle data_0024377c;
#ifdef __cplusplus
}
#endif

#endif
