#ifndef GAME_ARCHIVE_IPC_H
#define GAME_ARCHIVE_IPC_H

struct ArchiveInfo;
struct ArchiveOutputCommand;
struct WordPair;

/* [INFERENCE] Filesystem token, distinct from the sound ArchiveHandle.
 * Both consecutive words are forwarded to the terminal IPC operation. */
struct FileArchiveHandle { unsigned long long value; };

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
extern unsigned func_0012d5c4(struct ArchiveInfo *output, unsigned char mode,
    const unsigned long long *ids, unsigned count);
extern unsigned func_00136340(void *output, unsigned char mode,
    unsigned long long id, const unsigned short *input, unsigned count);
extern CtrResult func_0012d550(unsigned *count, struct ArchiveInfo *entries,
    unsigned capacity, unsigned char mode, unsigned long long token,
    unsigned index);
extern CtrResult func_0011f6a8(unsigned *count, struct ArchiveInfo *entries,
    unsigned capacity, unsigned char mode, unsigned long long token,
    unsigned index);
extern struct WordPair *func_00117ad8(struct WordPair *result,
    const struct FileArchiveHandle *archive, unsigned *count,
    struct ArchiveInfo *entries, unsigned capacity, unsigned index);
#ifdef __cplusplus
}
#endif

#endif
