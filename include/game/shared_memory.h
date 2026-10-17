#ifndef GAME_SHARED_MEMORY_H
#define GAME_SHARED_MEMORY_H
#include "ctr/svc.h"
struct SharedMemory {
    unsigned reserved00[2];
    unsigned size;
    unsigned char reserved0c[0x74];
#ifndef __cplusplus
    unsigned char memory[];
#endif
};
/* The former standalone pointer at +0 and the mapping handle at +4 belong
 * to this same mapping record. The second symbol names its handle word. */
struct SharedMemoryMapping {
    struct SharedMemory *address;
    CtrHandle handle;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct SharedMemoryMapping data_00242d88;
extern CtrHandle data_00242d8c;
extern CtrResult func_001151b8(CtrHandle *handle, struct SharedMemory **address);
extern CtrResult func_0010bf88(void);
extern unsigned func_0013e71c(void);
extern unsigned char *func_0019dc04(void);
#ifdef __cplusplus
}
#endif
#endif
