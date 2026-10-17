#include "game/shared_memory.h"

extern void func_0019ecf8(void);
extern CtrResult func_0012349c(CtrHandle *handle, struct SharedMemory **address);
extern void func_0019ec88(void);

CtrResult func_001151b8(CtrHandle *handle, struct SharedMemory **address)
{
    CtrResult result;
    func_0019ecf8();
    result = func_0012349c(handle, address);
    func_0019ec88();
    return result;
}
