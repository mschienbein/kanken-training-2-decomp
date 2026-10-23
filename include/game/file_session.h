#ifndef GAME_FILE_SESSION_H
#define GAME_FILE_SESSION_H
#include "ctr/svc.h"
#include "game/stream_manager.h"
struct FileSessionVTable;
struct FileObjectPool;
struct FileService;
struct FileSessionObject {
    union {
        const struct FileSessionVTable *vtable;
        struct FileSessionObject *next_free;
    };
    CtrHandle handle;
};
/* Original vtable entries identify these two distinct lifecycle protocols.
 * The unused slots remain opaque rather than acquiring invented signatures. */
struct FileSessionVTable {
    unsigned char reserved00[0x30];
    void (*release)(struct FileSessionObject *object);
    struct FileSessionObject *(*reset)(struct FileSessionObject *object);
};
struct FileObjectPool {
    unsigned char reserved00[0x24];
    struct FileSessionObject *free;
    unsigned reserved28;
    unsigned allocated;
    struct StreamMutex mutex;
};
#ifdef __cplusplus
/* [INFERENCE] Separate pool at bss_004b1cec; the reset callback and reuse
 * overlay are observed, not aliases for bss_004b1d28's session object. */
struct GlobalPoolObject;
struct GlobalPoolVTable {
    unsigned char reserved00[0x34];
    void (*reset)(GlobalPoolObject *object);
};
struct GlobalPoolObject {
    union {
        const GlobalPoolVTable *vtable;
        GlobalPoolObject *next_free;
    };
    void release();
};
struct GlobalObjectFreeList {
    unsigned char reserved00[0x24];
    GlobalPoolObject *free_head;
    unsigned capacity;
    int used;
    struct StreamMutex mutex;
};
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern struct FileObjectPool bss_004b1d28;
extern const struct FileSessionVTable data_00223af4;
extern struct FileSessionObject *func_00192978(struct FileObjectPool *pool);
extern struct FileSessionObject *func_001c4d1c(struct FileSessionObject *object);
extern void func_001c4bdc(struct FileSessionObject *object);
extern CtrResult func_001c4d9c(struct FileService *service,
    struct FileSessionObject **output, CtrHandle handle);
/* The checked-handle getter receives the same factory-backed session object;
 * its first word is the canonical vtable, not an unrelated integer prefix. */
extern unsigned func_0016bd7c(struct FileSessionObject *object);
extern CtrResult func_001c4f80(const CtrHandle *session);
#ifdef __cplusplus
extern GlobalObjectFreeList bss_004b1cec;
#endif
#ifdef __cplusplus
}
#endif
#endif
