#ifndef GAME_FILE_SESSION_H
#define GAME_FILE_SESSION_H
#include "ctr/svc.h"
struct FileSessionVTable;
struct FileObjectPool;
struct FileService;
struct FileSessionObject {
    const struct FileSessionVTable *vtable;
    CtrHandle handle;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct FileObjectPool bss_004b1d28;
extern const struct FileSessionVTable data_00223af4;
extern struct FileSessionObject *func_00192978(struct FileObjectPool *pool);
extern struct FileSessionObject *func_001c4d1c(struct FileSessionObject *object);
extern CtrResult func_001c4d9c(struct FileService *service,
    struct FileSessionObject **output, CtrHandle handle);
#ifdef __cplusplus
}
#endif
#endif
