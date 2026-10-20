#include "game/archive_ipc.h"

/* The terminal operation consumes one complete 64-bit token, not three
 * independently typed trailing words. Writable outputs retain their types. */
CtrResult func_0011f6a8(unsigned *output, struct ArchiveInfo *buffer, unsigned count,
                        unsigned char mode, unsigned long long token,
                        unsigned index)
{
    return func_0012d550(output, buffer, count, mode, token, index);
}
